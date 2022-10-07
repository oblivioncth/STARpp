#include <QCoreApplication>

// Core Includes
#include "star/election.h"
#include "star/reference.h"
#include "star/calculator.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    // Handle command line options...

    // Just dummy paths for now
    QString ccPath = "C:/Users/Chris/Desktop/test_config.ini";
    QString bbPath = "C:/Users/Chris/Desktop/test_box.csv";

    QList<Star::Election> elections;
    Qx::GenericError electionLoadError = Star::electionsFromReferenceInput(elections, ccPath, bbPath);
    if(electionLoadError.isValid())
    {

    }

    // Create calculator
    Star::Calculator calculator;

    // Result container
    QList<Star::ElectionResult> results;

    // Calculate the results of each election
    for(const Star::Election& election : elections)
        results.append(calculator.calculateResult(election));

    // Display results...

    return 0;
}
