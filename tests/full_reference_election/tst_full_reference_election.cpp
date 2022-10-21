// Qt Includes
#include <QtTest>

// Base Includes
#include <star/reference.h>
#include <star/calculator.h>

class tst_full_reference_election : public QObject
{
    Q_OBJECT

public:
    tst_full_reference_election();

private slots:
    // Init
//    void initTestCase();
//    void cleanupTestCase();

    // Test cases
    void basic_election_sampler();

};

tst_full_reference_election::tst_full_reference_election() {}
//void tst_full_reference_election::initTestCase() {}
//void tst_full_reference_election::cleanupTestCase() {}

void tst_full_reference_election::basic_election_sampler()
{    
    // Load expected results
    const QString erPath = QStringLiteral(":/data/basic_election_sampler.json");

    QList<Star::ExpectedElectionResult> expectedResults;
    Star::ReferenceError expectedResultsLoadError = Star::expectedResultsFromReferenceInput(expectedResults, erPath);
    QVERIFY2(!expectedResultsLoadError.isValid(), expectedResultsLoadError.errorDetails.toStdString().c_str());

    // Load reference elections
    const QString ccPath = QStringLiteral(":/data/basic_election_sampler.ini");
    const QString bbPath = QStringLiteral(":/data/basic_election_sampler.csv");

    QList<Star::Election> elections;
    Star::ReferenceError electionLoadError = Star::electionsFromReferenceInput(elections, ccPath, bbPath);
    QVERIFY2(!electionLoadError.isValid(), electionLoadError.errorDetails.toStdString().c_str());

    // Create calculator
    Star::Calculator calculator;
    calculator.setExtraTiebreakMethod(std::nullopt);

    // Check the results of each election
    for(qsizetype i = 0; i < elections.size(); i++)
    {
        const Star::Election& election = elections.at(i);
        calculator.setElection(&election);
        Star::ElectionResult result = calculator.calculateResult();
        QCOMPARE(result, expectedResults.at(i));
    }
}

QTEST_APPLESS_MAIN(tst_full_reference_election)
#include "tst_full_reference_election.moc"
