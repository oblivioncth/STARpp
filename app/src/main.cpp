// Qt Includes
#include <QCoreApplication>

// Base Includes
#include "star/election.h"
#include "star/reference.h"
#include "star/calculator.h"

// Project Includes
#include "core.h"
#include "project_vars.h"

// Meta
const QString NAME = QStringLiteral("Main");

int main(int argc, char *argv[])
{
    // Setup application
    QCoreApplication app(argc, argv);
    app.setApplicationName(PROJECT_APP_NAME);
    app.setApplicationVersion(PROJECT_VERSION_STR);

    // Create core and initialize it
    Core core(&app);
    ErrorCode initError = core.initialize();
    if(initError)
        return core.logFinish(initError);

    // Check if election was provided
    if(!core.hasActionableArguments())
        return core.logFinish(ErrorCode::NO_ERR);

    // Load reference election
    ReferenceElectionConfig rec = core.referenceElectionConfig();

    QList<Star::Election> elections;
    Qx::GenericError electionLoadError = Star::electionsFromReferenceInput(elections, rec.ccPath, rec.bbPath);
    if(electionLoadError.isValid())
    {
        core.postError(NAME, electionLoadError);
        return core.logFinish(ErrorCode::INVALID_REF_ELECTION);
    }

    // Create calculator
    Star::Calculator calculator;
    QObject::connect(&calculator, &Star::Calculator::calculationDetailDetermined, &core, &Core::logCalculatorDetail);

    // Result container
    QList<Star::ElectionResult> results;

    // Calculate the results of each election
    for(const Star::Election& election : elections)
        results.append(calculator.calculateResult(election));

    // Display results...

    return 0;
}
