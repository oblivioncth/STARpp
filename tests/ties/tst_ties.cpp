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

    //################################### PRELIMINARY ####################################################

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
               Star::Calculator::NoOptions
    );





    addTestRow("Preliminary - First place N-way tie, successful break",
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
               Star::ExpectedElectionResult({candidate1}, {candidate4}),
               Star::Calculator::NoOptions
    );

    addTestRow("Preliminary - First place N-way tie, unsuccessful break",
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
               Star::ExpectedElectionResult({candidate1}, {candidate4}),
               Star::Calculator::NoOptions
    );

    addTestRow("Preliminary - Second place N-way tie, successful break",
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
               Star::ExpectedElectionResult({candidate3}, {candidate1}),
               Star::Calculator::NoOptions
    );

    addTestRow("Preliminary - Second place N-way tie, unsuccessful break",
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
               Star::ExpectedElectionResult({candidate3}, {candidate1}),
               Star::Calculator::NoOptions
    );

    //################################### RUNOFF ####################################################

    addTestRow("Runoff - First place N-way tie, successful break",
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
               Star::ExpectedElectionResult({candidate3}, {candidate1}),
               Star::Calculator::NoOptions
    );

    addTestRow("Runoff - First place N-way tie, unsuccessful break, single fallback for second",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3}),
               Star::Calculator::NoOptions
    );

    addTestRow("Runoff - First place N-way tie, unsuccessful break, tied fallback for second that successfully breaks",
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
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3}),
               Star::Calculator::NoOptions
    );

    addTestRow("Runoff - First place N-way tie, unsuccessful break, tied fallback for second that doesn't break",
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
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3, candidate4}),
               Star::Calculator::NoOptions
    );

    addTestRow("Runoff - First place N-way tie, unsuccessful break, no fallback for second",
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
               Star::ExpectedElectionResult({candidate3, candidate4}, {}),
               Star::Calculator::NoOptions
    );

    addTestRow("Runoff - Second place N-way tie, successful break",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate3}, {candidate1}),
               Star::Calculator::NoOptions
    );

    addTestRow("Runoff - Second place N-way tie, unsuccessful break",
               {
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3, candidate4}),
               Star::Calculator::NoOptions
    );

    //################################### EXTENDED - FIVESTAR ####################################################

    addTestRow("Extended [FiveStar] - First place N-way tie, successful break",
               {
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 1},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2}, {candidate1}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [FiveStar] - First place N-way tie, unsuccessful break, single fallback for second",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3}),
               Star::Calculator::NoOptions
    );

        addTestRow("Extended [FiveStar] - First place N-way tie, unsuccessful break, tied fallback for second that successfully breaks",
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
                   Star::ExpectedElectionResult({candidate2, candidate1}, {candidate4}),
                   Star::Calculator::NoOptions
        );

    addTestRow("Extended [FiveStar] - First place N-way tie, unsuccessful break, tied fallback for second that doesn't break",
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
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3, candidate4}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [FiveStar] - First place N-way tie, unsuccessful break, no fallback for second",
               {
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1, candidate4}, {}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [FiveStar] - Second place N-way tie, successful break",
               {
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 4}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 4}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 0},
                       {.candidate = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [FiveStar] - Second place N-way tie, unsuccessful break",
               {
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3, candidate4}),
               Star::Calculator::NoOptions
    );

    //################################### EXTENDED - HEAD-TO-HEAD-WINS ####################################################

    addTestRow("Extended [HTHWins] - First place N-way tie, successful break",
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
               Star::ExpectedElectionResult({candidate1}, {candidate2}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHWins] - First place N-way tie, unsuccessful break, single fallback for second",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHWins] - First place N-way tie, unsuccessful break, tied fallback for second that successfully breaks",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 2},
                       {.candidate = candidate5, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2},
                       {.candidate = candidate5, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 1},
                       {.candidate = candidate5, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 5},
                       {.candidate = candidate5, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 5},
                       {.candidate = candidate5, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 0},
                       {.candidate = candidate4, .score = 0},
                       {.candidate = candidate5, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate5}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHWins] - First place N-way tie, unsuccessful break, tied fallback for second that doesn't break",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 2},
                       {.candidate = candidate5, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2},
                       {.candidate = candidate5, .score = 4}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 1},
                       {.candidate = candidate5, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 5},
                       {.candidate = candidate5, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 5},
                       {.candidate = candidate5, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 0},
                       {.candidate = candidate4, .score = 0},
                       {.candidate = candidate5, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3, candidate5}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHWins] - First place N-way tie, unsuccessful break, no fallback for second",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 0}
                   },
                   {
                       {.candidate = candidate1, .score = 1},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 0}
                   },
                   {
                       {.candidate = candidate1, .score = 1},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate3, candidate1}, {}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHWins] - Second place N-way tie, successful break",
               {
                   {
                       {.candidate = candidate1, .score = 1},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 4},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 4},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate3}, {candidate1}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHWins] - Second place N-way tie, unsuccessful break",
               {
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3, candidate4}),
               Star::Calculator::NoOptions
    );

    //################################### EXTENDED - HEAD-TO-HEAD PREF COUNT ####################################################

    addTestRow("Extended [HTHCount] - First place N-way tie, successful break",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate2}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHCount] - First place N-way tie, unsuccessful break, single fallback for second",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 4}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 4}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 0},
                       {.candidate = candidate4, .score = 4}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate3, candidate4}, {candidate1}),
               Star::Calculator::NoOptions
    );

    // TODO: addTestRow("Extended [HTHCount] - First place N-way tie, unsuccessful break, tied fallback for second that successfully breaks",
    // TODO: addTestRow("Extended [HTHCount] - First place N-way tie, unsuccessful break, tied fallback for second that doesn't break",

    addTestRow("Extended [HTHCount] - First place N-way tie, unsuccessful break, no fallback for second",
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
               Star::ExpectedElectionResult({candidate3, candidate4}, {}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHCount] - Second place N-way tie, successful break",
               {
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHCount] - Second place N-way tie, unsuccessful break",
               {
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 4}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 4}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 0},
                       {.candidate = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate4, candidate3}),
               Star::Calculator::NoOptions
    );

    //################################### EXTENDED - HEAD-TO-HEAD MARGIN ####################################################

    addTestRow("Extended [HTHMargin] - First place N-way tie, successful break",
               {
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 2},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 0},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate2}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHMargin] - First place N-way tie, unsuccessful break, no fallback for second",
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
               Star::ExpectedElectionResult({candidate3, candidate4}, {}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHMargin] - First place N-way tie, unsuccessful break, single fallback for second",
               {
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 0}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 4},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 1},
                       {.candidate = candidate2, .score = 3},
                       {.candidate = candidate3, .score = 0},
                       {.candidate = candidate4, .score = 5}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate3, candidate4}, {candidate1}),
               Star::Calculator::NoOptions
    );

    // TODO: addTestRow("Extended [HTHMargin] - First place N-way tie, unsuccessful break, tied fallback for second that successfully breaks",
    // TODO: addTestRow("Extended [HTHMargin] - First place N-way tie, unsuccessful break, tied fallback for second that doesn't break",

    addTestRow("Extended [HTHMargin] - Second place N-way tie, successful break",
               {
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 3}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 3},
                       {.candidate = candidate4, .score = 1}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3}),
               Star::Calculator::NoOptions
    );

    addTestRow("Extended [HTHMargin] - Second place N-way tie, unsuccessful break",
               {
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 5},
                       {.candidate = candidate3, .score = 1},
                       {.candidate = candidate4, .score = 4}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 1},
                       {.candidate = candidate3, .score = 5},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 4},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 2},
                       {.candidate = candidate4, .score = 2}
                   },
                   {
                       {.candidate = candidate1, .score = 3},
                       {.candidate = candidate2, .score = 2},
                       {.candidate = candidate3, .score = 4},
                       {.candidate = candidate4, .score = 4}
                   },
                   {
                       {.candidate = candidate1, .score = 5},
                       {.candidate = candidate2, .score = 0},
                       {.candidate = candidate3, .score = 0},
                       {.candidate = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate4, candidate3}),
               Star::Calculator::NoOptions
    );

    //################################### EXTENDED - RANDOM ####################################################

    /* NOTE: A separate test for this would need to be made if the lib could be accessed with fine enough
     * granularity to check just the result of the tiebreak itself, though that would be really jank. The method
     * is simple enough and always results in a winner, with all other candidates that were part of the break being
     * set to runner-up, so it should be ok.
     *
     * Otherwise, a separate case within this test target could be made that just ensures that the results
     * after using the method several times is fairly random (i.e. check for at least 2-3 different results).
     */
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
