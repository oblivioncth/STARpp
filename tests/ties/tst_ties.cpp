// Qt Includes
#include <QtTest>

// Base Includes
#include <star/reference.h>
#include <star/calculator.h>

// Macros
#define C_STR(q_str) q_str.toStdString().c_str()

class tst_ties : public QObject
{
    Q_OBJECT

public:
    tst_ties();

private slots:
    // Init
//    void initTestCase();
//    void cleanupTestCase();

    // Test cases
    void all_tie_cases_data();
    void all_tie_cases();

};

tst_ties::tst_ties() {}
//void tst_ties::initTestCase() {}
//void tst_ties::cleanupTestCase() {}

void tst_ties::all_tie_cases_data()
{
    // Setup test table
    QTest::addColumn<Star::Election>("election");
    QTest::addColumn<Star::ExpectedElectionResult>("expected_result");
    QTest::addColumn<std::optional<Star::Calculator::ExtendedTiebreakMethod>>("extended_method");

    // Candidates
    QString candidateOne = "CanOne";
    QString candidateTwo = "CanTwo";
    QString candidateThree = "CanThree";
    QString candidateFour = "CanFour";

    // Helper
    auto addTestRow = [&](const QString& testName,
                          QList<QList<Star::Election::Vote>> votes,
                          Star::ExpectedElectionResult results,
                          std::optional<Star::Calculator::ExtendedTiebreakMethod> extendedMethod){

        static Star::Election::Builder builder("");

        for(QList<Star::Election::Vote> voteList : votes)
            builder.wBallot({}, voteList);

        Star::Election election = builder.build();

        QTest::newRow(C_STR(testName)) << election << results << extendedMethod;
        builder.reset();
    };

    //-Populate test table rows with each case-----------------------------

    addTestRow("Preliminary - First 2-way place tie",
               {
                   {
                       {.nominee = candidateOne, .score = 0},
                       {.nominee = candidateTwo, .score = 4},
                       {.nominee = candidateThree, .score = 0},
                       {.nominee = candidateFour, .score = 0}
                   },
                   {
                       {.nominee = candidateOne, .score = 2},
                       {.nominee = candidateTwo, .score = 1},
                       {.nominee = candidateThree, .score = 3},
                       {.nominee = candidateFour, .score = 0}
                   },
                   {
                       {.nominee = candidateOne, .score = 3},
                       {.nominee = candidateTwo, .score = 0},
                       {.nominee = candidateThree, .score = 1},
                       {.nominee = candidateFour, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidateOne}, {candidateTwo}),
               std::nullopt
    );
}

void tst_ties::all_tie_cases()
{
    // Fetch data from test table
    QFETCH(Star::Election, election);
    QFETCH(Star::ExpectedElectionResult, expected_result);
    QFETCH(std::optional<Star::Calculator::ExtendedTiebreakMethod>, extended_method);

    // Setup calculator
    static Star::Calculator calc;
    calc.setElection(&election);
    calc.setExtraTiebreakMethod(extended_method);

    // Compare results
    QCOMPARE(calc.calculateResult(), expected_result);
}

QTEST_APPLESS_MAIN(tst_ties)
#include "tst_ties.moc"
