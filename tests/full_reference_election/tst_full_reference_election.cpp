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
    // Expected results
    QList<QPair<QStringList, QStringList>> expectedResults
    {
        {{"Sixth film"},{"Eighth film"}},
        {{"Fourth Director"},{"Fifth Director"}},
        {{"Fifth Cheesesteak"},{"Fourth Cheesesteak"}},
        {{"Sixth Actress"},{"Fifth Actress"}},
        {{"Fifth Actor"},{"First Actor"}},
        {{"First Supporting Actress"},{"Fourth Supporting Actress"}},
        {{"First Supporting Actor"},{"Second Supporting Actor"}},
        {{"Fifth Documentary"},{"Second Documentary"}},
        {{"Third Script"},{"Second Script"}},
        {{"Fifth Soundtrack"},{"Third Soundtrack"}},
        {{"Fourth Directorial Debut"},{"Fifth Directorial Debut"}},
        {{"Fourth Breakthrough"},{"First Breakthrough"}},
        {{"Fourth Cinematography"},{"Fifth Cinematography"}},
        {{"Second Foreign"},{"Fourth Foreign"}},
        {{"Second Animated"},{"First Animated"}},
        {{"Third Ensemble"},{"Second Ensemble"}},
        {{"Second Friedman"},{"Third Friedman"}},
        {{"Second May"},{"First May"}},
    };

    // Load reference elections
    const QString ccPath = QStringLiteral(":/data/basic_election_sampler.ini");
    const QString bbPath = QStringLiteral(":/data/basic_election_sampler.csv");

    QList<Star::Election> elections;
    Qx::GenericError electionLoadError = Star::electionsFromReferenceInput(elections, ccPath, bbPath);
    QVERIFY2(!electionLoadError.isValid(), electionLoadError.secondaryInfo().toStdString().c_str());

    // Create calculator
    Star::Calculator calculator;
    calculator.setExtraTiebreak(false);

    // Check the results of each election
    for(qsizetype i = 0; i < elections.size(); i++)
    {
        const Star::Election& election = elections.at(i);
        calculator.setElection(&election);
        Star::ElectionResult result = calculator.calculateResult();
        QPair<QStringList, QStringList> resultPair(result.winners(), result.runnerUps());
        QCOMPARE(resultPair, expectedResults.at(i));
    }
}

QTEST_APPLESS_MAIN(tst_full_reference_election)
#include "tst_full_reference_election.moc"
