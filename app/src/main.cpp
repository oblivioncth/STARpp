// Qt Includes
#include <QCoreApplication>

// Qx Includes
#include <qx/core/qx-iostream.h>
#include <qx/core/qx-abstracterror.h>

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

// Msg
const QString MSG_CALCULING_ELECTION_RESULTS = QStringLiteral("Calculating election results...");

// Meta
const QString NAME = QStringLiteral("Main");

// Ref Error adapter
class QX_ERROR_TYPE(ReferenceErrorAdapter, "Star::ReferenceError", 1201)
{
private:
    const Star::ReferenceError& mErrorRef;

public:
    ReferenceErrorAdapter(const Star::ReferenceError& e) :
        mErrorRef(e)
    {}
    ReferenceErrorAdapter(ReferenceErrorAdapter&&) = delete;
    ReferenceErrorAdapter(const ReferenceErrorAdapter&) = delete;

private:
    quint32 deriveValue() const override { return static_cast<quint32>(mErrorRef.type); }
    QString derivePrimary() const override { return mErrorRef.error; }
    QString deriveSecondary() const override { return mErrorRef.errorDetails; }
};
QX_DECLARE_ERROR_ADAPTATION(Star::ReferenceError, ReferenceErrorAdapter);

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
    Qx::Error initError = core.initialize();
    if(initError.isValid())
        return core.logFinish(initError);

    // Check if election was provided
    if(!core.hasActionableArguments())
    {
        core.logEvent(NAME, LOG_EVENT_NO_ELECTION);
        return core.logFinish(Qx::Error());
    }

    // Load reference election
    core.logEvent(NAME, LOG_EVENT_LOADING_ELECTION);
    ReferenceElectionConfig rec = core.referenceElectionConfig();

    QList<Star::Election> elections;
    Star::ReferenceError refError = Star::electionsFromReferenceInput(elections, rec.ccPath, rec.bbPath);
    if(refError.isValid())
    {
        core.postError(NAME, refError);
        return core.logFinish(refError);
    }
    core.logEvent(NAME, LOG_EVENT_ELECTION_COUNT.arg(elections.size()));

    // Create calculator
    Star::Calculator calculator;
    calculator.setOptions(core.calculatorOptions());
    QObject::connect(&calculator, &Star::Calculator::calculationDetail, &core, &Core::logCalculatorDetail);

    // Result container
    QList<Star::ElectionResult> results;

    // Calculate the results of each election
    core.logEvent(NAME, LOG_EVENT_CALCULATING_RESULTS);
    core.postMessage(MSG_CALCULING_ELECTION_RESULTS + '\n');
    for(const Star::Election& election : elections)
    {
        calculator.setElection(&election);
        results.append(calculator.calculateResult());
    }

    // Display results
    core.logEvent(NAME, LOG_EVENT_DISPLAYING_RESULTS);
    ResultPresenter presenter(&results, core.isMinimalPresentation());
    presenter.present();

    return core.logFinish(Qx::Error());
}
