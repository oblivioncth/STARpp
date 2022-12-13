// Qt Includes
#include <QtTest>

// Base Includes
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
    QTest::addColumn<Star::Calculator::Options>("calc_options");

    // Candidates
    QString candidate1 = "CanOne";
    QString candidate2 = "CanTwo";
    QString candidate3 = "CanThree";
    QString candidate4 = "CanFour";
    QString candidate5 = "CanFive";

    // Helper
    auto addTestRow = [&](const QString& testName,
                          QList<QList<Star::Election::Vote>> votes,
                          Star::ExpectedElectionResult results,
                          Star::Calculator::Options calcOptions){

        Star::Election::Builder builder(testName);

        for(const QList<Star::Election::Vote>& voteList : votes)
            builder.wBallot({}, voteList);

        builder.wSeatCount(1);
        Star::Election election = builder.build();

        QTest::newRow(C_STR(testName)) << election << results << calcOptions;
    };

    //-Populate test table rows with each case-----------------------------

    addTestRow("Scoring Round - First place 2-way tie",
               {
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 4},
                       {.candidate = candidate3, .score = 0},
                       {.candidate = candidate4, .score = 0}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 0}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate1}),
               Star::Calculator::AllowTrueTies
    );

    addTestRow("Scoring Round - First place 3-way tie, breaks from most head-to-head losses",
               {
                   {
                       {.candidate = candidate1, .score = 1},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 4},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 4},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}),
               Star::Calculator::AllowTrueTies
               );

    addTestRow("Scoring Round - First place 3-way tie, breaks from least five star",
               {
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 4},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}),
               Star::Calculator::AllowTrueTies
    );

    addTestRow("Scoring Round - Second place 4-way tie, breaks from least head-to-head preferences",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 3},
                       {.candidate = candidate5, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 3},
                       {.candidate = candidate5, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 3},
                       {.candidate = candidate5, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 5},
                       {.candidate = candidate5, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 2},
                       {.candidate = candidate5, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 4},
                       {.candidate = candidate5, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate3}),
               Star::Calculator::AllowTrueTies | Star::Calculator::CondorcetProtocol
               );

    // TODO: addTestRow("Scoring Round - ..., breaks from least head-to-head preferences", ...

    addTestRow("Scoring Round - Second place 3-way tie, never breaks (with Condorcet)",
               {
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 4}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 4},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 3}
                   }
               },
               Star::ExpectedElectionResult({}, {candidate1, candidate2, candidate3}),
               Star::Calculator::AllowTrueTies | Star::Calculator::CondorcetProtocol
               );

    addTestRow("Scoring Round - Second place 3-way tie, never breaks (without Condorcet)",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 3},
                       {.candidate = candidate5, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 3},
                       {.candidate = candidate5, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 3},
                       {.candidate = candidate5, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 5},
                       {.candidate = candidate5, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 2},
                       {.candidate = candidate5, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 4},
                       {.candidate = candidate5, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({}, {{candidate1, candidate3, candidate4}}),
               Star::Calculator::AllowTrueTies
               );

    addTestRow("Head-to-head narrowing test (multiple tied candidates to cut in scoring round)",
               {
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 0}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 4},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate3}),
               Star::Calculator::AllowTrueTies | Star::Calculator::CondorcetProtocol
               );

    addTestRow("Runoff - Tie, breaks from original score",
               {
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 4},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 3}
                   }
               },
               Star::ExpectedElectionResult({candidate3}),
               Star::Calculator::AllowTrueTies
               );

    addTestRow("Runoff - Tie, breaks from five star",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 3},
                       {.candidate = candidate5, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 3},
                       {.candidate = candidate5, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 3},
                       {.candidate = candidate5, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 5},
                       {.candidate = candidate5, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 2},
                       {.candidate = candidate5, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 4},
                       {.candidate = candidate5, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate1}),
               Star::Calculator::AllowTrueTies
               );

    addTestRow("Runoff - Tie, never breaks",
               {
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 4},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({}, {candidate3, candidate4}),
               Star::Calculator::AllowTrueTies
               );
}

void tst_ties::all_tie_cases()
{
    // Fetch data from test table
    QFETCH(Star::Election, election);
    QFETCH(Star::ExpectedElectionResult, expected_result);
    QFETCH(Star::Calculator::Options, calc_options);

    // Setup calculator
    static Star::Calculator calc;
    calc.setElection(&election);
    calc.setOptions(calc_options);

    // Compare results
    QCOMPARE(calc.calculateResult(), expected_result);
}

QTEST_APPLESS_MAIN(tst_ties)
#include "tst_ties.moc"
