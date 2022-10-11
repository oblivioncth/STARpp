// Qt Includes
#include <QCoreApplication>

// Qx Includes
#include <qx/core/qx-iostream.h>

// Base Includes
#include "star/election.h"
#include "star/reference.h"
#include "star/calculator.h"

// Project Includes
#include "core.h"
#include "project_vars.h"
#include "resultspresenter.h"

// Log
const QString LOG_EVENT_NO_ELECTION = QStringLiteral("No election data provided. Exiting...");
const QString LOG_EVENT_LOADING_ELECTION = QStringLiteral("Loading reference election data.");
const QString LOG_EVENT_ELECTION_COUNT = QStringLiteral("Loaded %1 elections.");
const QString LOG_EVENT_CALCULATING_RESULTS = QStringLiteral("Calculating results of all elections...");
const QString LOG_EVENT_DISPLAYING_RESULTS = QStringLiteral("Displaying results...");

// Meta
const QString NAME = QStringLiteral("Main");

int main(int argc, char *argv[])
{
    // Setup application
    QCoreApplication app(argc, argv);
    app.setApplicationName(PROJECT_APP_NAME);
    app.setApplicationVersion(PROJECT_VERSION_STR);

    // Disable console input echo since this application isn't interactive (prevents accidental console clutter)
    Qx::setUserInputEchoEnabled(false);
    QScopeGuard inputEchoGuard([]{ Qx::setUserInputEchoEnabled(true); }); // Re-enable before app finishes

    // Create core and initialize it
    Core core(&app);
    ErrorCode initError = core.initialize();
    if(initError)
        return core.logFinish(initError);

    // Check if election was provided
    if(!core.hasActionableArguments())
    {
        core.logEvent(NAME, LOG_EVENT_NO_ELECTION);
        return core.logFinish(ErrorCode::NO_ERR);
    }

    // Load reference election
    core.logEvent(NAME, LOG_EVENT_LOADING_ELECTION);
    ReferenceElectionConfig rec = core.referenceElectionConfig();

    QList<Star::Election> elections;
    Qx::GenericError electionLoadError = Star::electionsFromReferenceInput(elections, rec.ccPath, rec.bbPath);
    if(electionLoadError.isValid())
    {
        core.postError(NAME, electionLoadError);
        return core.logFinish(ErrorCode::INVALID_REF_ELECTION);
    }
    core.logEvent(NAME, LOG_EVENT_ELECTION_COUNT.arg(elections.size()));

    // Create calculator
    Star::Calculator calculator(rec.extraTiebreak);
    QObject::connect(&calculator, &Star::Calculator::calculationDetailDetermined, &core, &Core::logCalculatorDetail);

    // Result container
    QList<Star::ElectionResult> results;

    // Calculate the results of each election
    core.logEvent(NAME, LOG_EVENT_CALCULATING_RESULTS);
    for(const Star::Election& election : elections)
        results.append(calculator.calculateResult(election));

    // Display results
    core.logEvent(NAME, LOG_EVENT_DISPLAYING_RESULTS);
    ResultPresenter presenter(&results);
    presenter.present();

    return core.logFinish(ErrorCode::NO_ERR);
}
