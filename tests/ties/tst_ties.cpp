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
        Star::ExpectedElectionResult::Builder()
               .wSeat(Star::Seat(candidate1, Star::QualifierResult({candidate1, candidate2})))
               .build(),
        Star::Calculator::AllowTrueTies
    );

    addTestRow("Scoring Round - First place N-way tie [Reaches loss-based advancement]",
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
        Star::ExpectedElectionResult::Builder()
               .wSeat(Star::Seat(candidate1, Star::QualifierResult({candidate1, candidate2})))
               .build(),
        Star::Calculator::AllowTrueTies
    );

    addTestRow("Scoring Round - First place N-way tie [Reaches 5-star-based advancement]",
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
        Star::ExpectedElectionResult::Builder()
               .wSeat(Star::Seat(candidate1, Star::QualifierResult({candidate1}, {candidate4})))
               .build(),
        Star::Calculator::AllowTrueTies
    );

    addTestRow("Scoring Round - Second place N-way tie [Reaches 5-star-based culling]",
        {
            {
                {.candidate = candidate1, .score = 5},
                {.candidate = candidate2, .score = 1},
                {.candidate = candidate3, .score = 1},
                {.candidate = candidate4, .score = 4},
            },
            {
                {.candidate = candidate1, .score = 1},
                {.candidate = candidate2, .score = 5},
                {.candidate = candidate3, .score = 0},
                {.candidate = candidate4, .score = 2},
            },
            {
                {.candidate = candidate1, .score = 0},
                {.candidate = candidate2, .score = 1},
                {.candidate = candidate3, .score = 3},
                {.candidate = candidate4, .score = 3},
            },
            {
                {.candidate = candidate1, .score = 3},
                {.candidate = candidate2, .score = 0},
                {.candidate = candidate3, .score = 2},
                {.candidate = candidate4, .score = 5},
            },
            {
                {.candidate = candidate1, .score = 0},
                {.candidate = candidate2, .score = 2},
                {.candidate = candidate3, .score = 3},
                {.candidate = candidate4, .score = 0},
            }
        },
        Star::ExpectedElectionResult::Builder()
               .wSeat(Star::Seat(candidate4, Star::QualifierResult({candidate4}, {candidate2})))
               .build(),
        Star::Calculator::AllowTrueTies
    );

    addTestRow("Scoring Round - First place N-way tie [Reaches preference-based culling]",
        {
            {
                {.candidate = candidate1, .score = 3},
                {.candidate = candidate2, .score = 0},
                {.candidate = candidate3, .score = 1},
                {.candidate = candidate4, .score = 0},
                {.candidate = candidate5, .score = 2}
            },
            {
                {.candidate = candidate1, .score = 3},
                {.candidate = candidate2, .score = 5},
                {.candidate = candidate3, .score = 0},
                {.candidate = candidate4, .score = 3},
                {.candidate = candidate5, .score = 2}
            },
            {
                {.candidate = candidate1, .score = 2},
                {.candidate = candidate2, .score = 1},
                {.candidate = candidate3, .score = 5},
                {.candidate = candidate4, .score = 0},
                {.candidate = candidate5, .score = 0}
            },
            {
                {.candidate = candidate1, .score = 0},
                {.candidate = candidate2, .score = 4},
                {.candidate = candidate3, .score = 2},
                {.candidate = candidate4, .score = 4},
                {.candidate = candidate5, .score = 0}
            },
            {
                {.candidate = candidate1, .score = 2},
                {.candidate = candidate2, .score = 2},
                {.candidate = candidate3, .score = 4},
                {.candidate = candidate4, .score = 5},
                {.candidate = candidate5, .score = 5}
            }
        },
        Star::ExpectedElectionResult::Builder()
               .wSeat(Star::Seat(candidate3, Star::QualifierResult({candidate3}, {candidate2})))
               .build(),
        Star::Calculator::AllowTrueTies | Star::Calculator::CondorcetProtocol
    );

    // TODO: addTestRow("Scoring Round - ... place N-way tie [Reaches preference-based advancement]",

    addTestRow("Scoring Round - Second place N-way tie [Reaches margin-based culling]",
        {
            {
                {.candidate = candidate1, .score = 5},
                {.candidate = candidate2, .score = 0},
                {.candidate = candidate3, .score = 0},
                {.candidate = candidate4, .score = 1},
                {.candidate = candidate5, .score = 0}
            },
            {
                {.candidate = candidate1, .score = 1},
                {.candidate = candidate2, .score = 5},
                {.candidate = candidate3, .score = 0},
                {.candidate = candidate4, .score = 5},
                {.candidate = candidate5, .score = 0}
            },
            {
                {.candidate = candidate1, .score = 1},
                {.candidate = candidate2, .score = 0},
                {.candidate = candidate3, .score = 5},
                {.candidate = candidate4, .score = 0},
                {.candidate = candidate5, .score = 4}
            },
            {
                {.candidate = candidate1, .score = 0},
                {.candidate = candidate2, .score = 1},
                {.candidate = candidate3, .score = 0},
                {.candidate = candidate4, .score = 2},
                {.candidate = candidate5, .score = 5}
            },
            {
                {.candidate = candidate1, .score = 0},
                {.candidate = candidate2, .score = 0},
                {.candidate = candidate3, .score = 2},
                {.candidate = candidate4, .score = 0},
                {.candidate = candidate5, .score = 1}
            },
            {
                {.candidate = candidate1, .score = 1},
                {.candidate = candidate2, .score = 2},
                {.candidate = candidate3, .score = 1},
                {.candidate = candidate4, .score = 0},
                {.candidate = candidate5, .score = 0}
            }
        },
        Star::ExpectedElectionResult::Builder()
               .wSeat(Star::Seat(candidate5, Star::QualifierResult({candidate5}, {candidate1})))
               .build(),
        Star::Calculator::AllowTrueTies | Star::Calculator::CondorcetProtocol
    );

    // TODO: addTestRow("Scoring Round - ... place N-way tie [Reaches margin-based advancement]",

    addTestRow("Scoring Round - Second place N-way tie [Reaches random-based advancement]",
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
        Star::ExpectedElectionResult::Builder()
               .wSeat(Star::Seat(Star::QualifierResult({candidate3}, {candidate1, candidate2})))
               .build(),
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
