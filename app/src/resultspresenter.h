#ifndef RESULT_PRESENTER_H
#define RESULT_PRESENTER_H

// Base Includes
#include "star/electionresult.h"

class ResultPresenter
{
//-Class Variables------------------------------------------------------------------------------------------------------
private:
    // Headings
    static inline const QString HEADING_RESULTS = QStringLiteral(
        "#########################################################\n"
        "#########################################################\n"
        "#########                                       #########\n"
        "#########                RESULTS                #########\n"
        "#########                                       #########\n"
        "#########################################################\n"
        "#########################################################"
    );

    static inline const QString HEADING_CATEGORY = QStringLiteral(
        "=========================================================\n"
        "   Category) %1                                          \n"
        "========================================================="
    );

    static inline const QString HEADING_NOMINEES = QStringLiteral(
        "Nominees\n"
        "--------"
    );

    static inline const QString HEADING_OUTCOME = QStringLiteral(
        "Outcome\n"
        "-------"
    );

    static inline const QString HEADING_SUMMARY = QStringLiteral(
        "<><><><><><><><><><><><><><><><><><><><><><><><><><><><>\n"
        "   Summary                                              \n"
        "<><><><><><><><><><><><><><><><><><><><><><><><><><><><>"
    );

    // Result Templates
    static inline const QString WINNER_SINGLE_TEMPLATE = QStringLiteral(R"(Winner: "%1")");
    static inline const QString WINNER_MULTI_TEMPLATE = QStringLiteral(R"(Winners: "%1")");
    static inline const QString RUNNERUP_SINGLE_TEMPLATE = QStringLiteral(R"(Runner-Up: "%1")");
    static inline const QString RUNNERUP_MULTI_TEMPLATE = QStringLiteral(R"(Runner-Ups: "%1")");

    static inline const QString RAW_SCORE_TEMPLATE = QStringLiteral(R"({"%1"} [%2])");

    // Summary
    static inline const QString SUMMARY_HEADING_CATEGORY = QStringLiteral(" Category ");
    static inline const QString SUMMARY_HEADING_WINNER = QStringLiteral(" Winner ");
    static inline const QString SUMMARY_HEADING_RUNNER_UP = QStringLiteral(" Runner-up ");
    static inline const QString SUMMARY_LIST_ITEM = QStringLiteral(R"( "%1" )");
    static inline const QChar SUMMARY_VERTICAL_SEP = '|';
    static inline const QChar SUMMARY_HORIZONTAL_SEP = '-';

    // Pause
    static inline const QString USER_PAUSE_PROMPT = QStringLiteral("Press ENTER to continue...");

//-Instance Variables------------------------------------------------------------------------------------------------------
private:
    const QList<Star::ElectionResult>* mResults;
    bool mSummaryOnly;

//-Constructor----------------------------------------------------------------------------------------------------------
public:
    ResultPresenter(const QList<Star::ElectionResult>* results, bool summaryOnly = false);

//-Instance Functions--------------------------------------------------------------------------------------------------
private:
    void pause();
    void printElectionResult(const Star::ElectionResult& result);
    void printMainHeading();
    void printResults();
    void printSummary();

public:
    void present();
};

#endif // RESULT_PRESENTER_H
