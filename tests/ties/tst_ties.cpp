// Qt Includes
#include <QtTest>

// Base Includes
#include <star/reference.h>
#include <star/calculator.h>

// Test Includes
#include <star_test_common.h>

// Test
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
    QString candidate1 = "CanOne";
    QString candidate2 = "CanTwo";
    QString candidate3 = "CanThree";
    QString candidate4 = "CanFour";

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

    addTestRow("Preliminary - First place 2-way tie",
               {
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 4},
                       {.nominee = candidate3, .score = 0},
                       {.nominee = candidate4, .score = 0}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 0}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate2}),
               std::nullopt
    );

    addTestRow("Preliminary - First place N-way tie, successful break,",
               {
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 4},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate4}),
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
