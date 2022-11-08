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
    QTest::addColumn<std::optional<Star::Calculator::ExtendedTiebreakMethod>>("extended_method");

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
                          std::optional<Star::Calculator::ExtendedTiebreakMethod> extendedMethod){

        Star::Election::Builder builder(testName);

        for(QList<Star::Election::Vote> voteList : votes)
            builder.wBallot({}, voteList);

        Star::Election election = builder.build();

        QTest::newRow(C_STR(testName)) << election << results << extendedMethod;
    };

    //-Populate test table rows with each case-----------------------------

    //################################### PRELIMINARY ####################################################

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

    addTestRow("Preliminary - First place N-way tie, successful break",
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

    addTestRow("Preliminary - First place N-way tie, unsuccessful break",
               {
                   {
                       {.nominee = candidate1, .score = 1},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 4},
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

    addTestRow("Preliminary - Second place N-way tie, successful break",
               {
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 0}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 4},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate3}, {candidate1}),
               std::nullopt
    );

    addTestRow("Preliminary - Second place N-way tie, unsuccessful break",
               {
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 4}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 4},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 3}
                   }
               },
               Star::ExpectedElectionResult({candidate3}, {candidate1}),
               std::nullopt
    );

    //################################### RUNOFF ####################################################

    addTestRow("Runoff - First place N-way tie, successful break",
               {
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 4},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 3}
                   }
               },
               Star::ExpectedElectionResult({candidate3}, {candidate1}),
               std::nullopt
    );

    addTestRow("Runoff - First place N-way tie, unsuccessful break, single fallback for second",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3}),
               std::nullopt
    );

    addTestRow("Runoff - First place N-way tie, unsuccessful break, tied fallback for second that successfully breaks",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 5},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 2},
                       {.nominee = candidate5, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 4},
                       {.nominee = candidate5, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3}),
               std::nullopt
    );

    addTestRow("Runoff - First place N-way tie, unsuccessful break, tied fallback for second that doesn't break",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 5},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 2},
                       {.nominee = candidate5, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 4},
                       {.nominee = candidate5, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3, candidate4}),
               std::nullopt
    );

    addTestRow("Runoff - First place N-way tie, unsuccessful break, no fallback for second",
               {
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 4},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate3, candidate4}, {}),
               std::nullopt
    );

    addTestRow("Runoff - Second place N-way tie, successful break",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate3}, {candidate1}),
               std::nullopt
    );

    addTestRow("Runoff - Second place N-way tie, unsuccessful break",
               {
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3, candidate4}),
               std::nullopt
    );

    //################################### EXTENDED - FIVESTAR ####################################################

    addTestRow("Extended [FiveStar] - First place N-way tie, successful break",
               {
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 1},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2}, {candidate1}),
               Star::Calculator::FiveStar
    );

    addTestRow("Extended [FiveStar] - First place N-way tie, unsuccessful break, single fallback for second",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3}),
               Star::Calculator::FiveStar
    );

        addTestRow("Extended [FiveStar] - First place N-way tie, unsuccessful break, tied fallback for second that successfully breaks",
                   {
                       {
                           {.nominee = candidate1, .score = 5},
                           {.nominee = candidate2, .score = 3},
                           {.nominee = candidate3, .score = 4},
                           {.nominee = candidate4, .score = 3},
                           {.nominee = candidate5, .score = 3}
                       },
                       {
                           {.nominee = candidate1, .score = 2},
                           {.nominee = candidate2, .score = 5},
                           {.nominee = candidate3, .score = 3},
                           {.nominee = candidate4, .score = 3},
                           {.nominee = candidate5, .score = 2}
                       },
                       {
                           {.nominee = candidate1, .score = 3},
                           {.nominee = candidate2, .score = 2},
                           {.nominee = candidate3, .score = 4},
                           {.nominee = candidate4, .score = 3},
                           {.nominee = candidate5, .score = 3}
                       },
                       {
                           {.nominee = candidate1, .score = 3},
                           {.nominee = candidate2, .score = 2},
                           {.nominee = candidate3, .score = 2},
                           {.nominee = candidate4, .score = 5},
                           {.nominee = candidate5, .score = 2}
                       },
                       {
                           {.nominee = candidate1, .score = 5},
                           {.nominee = candidate2, .score = 3},
                           {.nominee = candidate3, .score = 4},
                           {.nominee = candidate4, .score = 2},
                           {.nominee = candidate5, .score = 5}
                       },
                       {
                           {.nominee = candidate1, .score = 2},
                           {.nominee = candidate2, .score = 5},
                           {.nominee = candidate3, .score = 3},
                           {.nominee = candidate4, .score = 4},
                           {.nominee = candidate5, .score = 5}
                       }
                   },
                   Star::ExpectedElectionResult({candidate2, candidate1}, {candidate4}),
                   Star::Calculator::FiveStar
        );

    addTestRow("Extended [FiveStar] - First place N-way tie, unsuccessful break, tied fallback for second that doesn't break",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 5},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 2},
                       {.nominee = candidate5, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 4},
                       {.nominee = candidate5, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3, candidate4}),
               Star::Calculator::FiveStar
    );

    addTestRow("Extended [FiveStar] - First place N-way tie, unsuccessful break, no fallback for second",
               {
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1, candidate4}, {}),
               Star::Calculator::FiveStar
    );

    addTestRow("Extended [FiveStar] - Second place N-way tie, successful break",
               {
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 4}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 4}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 0},
                       {.nominee = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3}),
               Star::Calculator::FiveStar
    );

    addTestRow("Extended [FiveStar] - Second place N-way tie, unsuccessful break",
               {
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3, candidate4}),
               Star::Calculator::FiveStar
    );

    //################################### EXTENDED - HEAD-TO-HEAD-WINS ####################################################

    addTestRow("Extended [HTHWins] - First place N-way tie, successful break",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 3},
                       {.nominee = candidate5, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 5},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 2},
                       {.nominee = candidate5, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 4},
                       {.nominee = candidate5, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate2}),
               Star::Calculator::HTHWins
    );

    addTestRow("Extended [HTHWins] - First place N-way tie, unsuccessful break, single fallback for second",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3}),
               Star::Calculator::HTHWins
    );

    addTestRow("Extended [HTHWins] - First place N-way tie, unsuccessful break, tied fallback for second that successfully breaks",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 2},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2},
                       {.nominee = candidate5, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 1},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 5},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 5},
                       {.nominee = candidate5, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 0},
                       {.nominee = candidate4, .score = 0},
                       {.nominee = candidate5, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate5}),
               Star::Calculator::HTHWins
    );

    addTestRow("Extended [HTHWins] - First place N-way tie, unsuccessful break, tied fallback for second that doesn't break",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 2},
                       {.nominee = candidate5, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2},
                       {.nominee = candidate5, .score = 4}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 1},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 5},
                       {.nominee = candidate5, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 5},
                       {.nominee = candidate5, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 0},
                       {.nominee = candidate4, .score = 0},
                       {.nominee = candidate5, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate1}, {candidate3, candidate5}),
               Star::Calculator::HTHWins
    );

    addTestRow("Extended [HTHWins] - First place N-way tie, unsuccessful break, no fallback for second",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 0}
                   },
                   {
                       {.nominee = candidate1, .score = 1},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 0}
                   },
                   {
                       {.nominee = candidate1, .score = 1},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate2, candidate3, candidate1}, {}),
               Star::Calculator::HTHWins
    );

    addTestRow("Extended [HTHWins] - Second place N-way tie, successful break",
               {
                   {
                       {.nominee = candidate1, .score = 1},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 4},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 4},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate3}, {candidate1}),
               Star::Calculator::HTHWins
    );

    addTestRow("Extended [HTHWins] - Second place N-way tie, unsuccessful break",
               {
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3, candidate4}),
               Star::Calculator::HTHWins
    );

    //################################### EXTENDED - HEAD-TO-HEAD PREF COUNT ####################################################

    addTestRow("Extended [HTHCount] - First place N-way tie, successful break",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate2}),
               Star::Calculator::HTHCount
    );

    addTestRow("Extended [HTHCount] - First place N-way tie, unsuccessful break, single fallback for second",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 4}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 4}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 0},
                       {.nominee = candidate4, .score = 4}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate3, candidate4}, {}),
               Star::Calculator::HTHCount
    );

    // TODO: addTestRow("Extended [HTHCount] - First place N-way tie, unsuccessful break, tied fallback for second that successfully breaks",
    // TODO: addTestRow("Extended [HTHCount] - First place N-way tie, unsuccessful break, tied fallback for second that doesn't break",

    addTestRow("Extended [HTHCount] - First place N-way tie, unsuccessful break, no fallback for second",
               {
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 4},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate3, candidate4}, {}),
               Star::Calculator::HTHCount
    );

    addTestRow("Extended [HTHCount] - Second place N-way tie, successful break",
               {
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3}),
               Star::Calculator::HTHCount
    );

    addTestRow("Extended [HTHCount] - Second place N-way tie, unsuccessful break",
               {
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 4}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 4}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 0},
                       {.nominee = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate4, candidate3}),
               Star::Calculator::HTHCount
    );

    //################################### EXTENDED - HEAD-TO-HEAD MARGIN ####################################################

    addTestRow("Extended [HTHMargin] - First place N-way tie, successful break",
               {
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 2},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 0},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate2}),
               Star::Calculator::HTHMargin
    );

    addTestRow("Extended [HTHMargin] - First place N-way tie, unsuccessful break, no fallback for second",
               {
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 4},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate3, candidate4}, {}),
               Star::Calculator::HTHMargin
    );

    addTestRow("Extended [HTHMargin] - First place N-way tie, unsuccessful break, single fallback for second",
               {
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 0}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 4},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 1},
                       {.nominee = candidate2, .score = 3},
                       {.nominee = candidate3, .score = 0},
                       {.nominee = candidate4, .score = 5}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 5}
                   }
               },
               Star::ExpectedElectionResult({candidate3, candidate4}, {candidate1}),
               Star::Calculator::HTHMargin
    );

    // TODO: addTestRow("Extended [HTHMargin] - First place N-way tie, unsuccessful break, tied fallback for second that successfully breaks",
    // TODO: addTestRow("Extended [HTHMargin] - First place N-way tie, unsuccessful break, tied fallback for second that doesn't break",

    addTestRow("Extended [HTHMargin] - Second place N-way tie, successful break",
               {
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 3}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 3},
                       {.nominee = candidate4, .score = 1}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 4}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate3}),
               Star::Calculator::HTHMargin
    );

    addTestRow("Extended [HTHMargin] - Second place N-way tie, unsuccessful break",
               {
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 5},
                       {.nominee = candidate3, .score = 1},
                       {.nominee = candidate4, .score = 4}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 1},
                       {.nominee = candidate3, .score = 5},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 4},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 2},
                       {.nominee = candidate4, .score = 2}
                   },
                   {
                       {.nominee = candidate1, .score = 3},
                       {.nominee = candidate2, .score = 2},
                       {.nominee = candidate3, .score = 4},
                       {.nominee = candidate4, .score = 4}
                   },
                   {
                       {.nominee = candidate1, .score = 5},
                       {.nominee = candidate2, .score = 0},
                       {.nominee = candidate3, .score = 0},
                       {.nominee = candidate4, .score = 0}
                   }
               },
               Star::ExpectedElectionResult({candidate1}, {candidate4, candidate3}),
               Star::Calculator::HTHMargin
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
