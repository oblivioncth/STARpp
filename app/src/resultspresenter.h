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

    static inline const QString HEADING_CANDIDATES = QStringLiteral(
        "Candidates\n"
        "--------"
    );

    static inline const QString HEADING_WINNERS = QStringLiteral(
        "Winners\n"
        "-------"
    );

    static inline const QString HEADING_UNRESOLVED = QStringLiteral(
        "UNRESOLVED\n"
        "----------"
    );

    static inline const QString HEADING_SCORE_RANKINGS = QStringLiteral(
        "Score Rankings\n"
        "--------------"
    );

    static inline const QString HEADING_SUMMARY = QStringLiteral(
        "<><><><><><><><><><><><><><><><><><><><><><><><><><><><>\n"
        "   Summary                                              \n"
        "<><><><><><><><><><><><><><><><><><><><><><><><><><><><>"
    );

    // Result Templates
    static inline const QString WINNER_TEMPLATE = QStringLiteral(R"(%1) "%2")");
    static inline const QString UNRESOLVED_TEMPLATE = QStringLiteral(R"(- "%1")");
    static inline const QString RAW_SCORE_TEMPLATE = QStringLiteral(R"({"%1"} [%2])");

    // Summary
    static inline const QString SUMMARY_HEADING_CATEGORY = QStringLiteral(" Category ");
    static inline const QString SUMMARY_HEADING_WINNER = QStringLiteral(" Winner ");
    static inline const QString SUMMARY_HEADING_SECOND_SEAT = QStringLiteral(" Second Seat ");
    static inline const QString SUMMARY_LIST_ITEM = QStringLiteral(" %1 ");
    static inline const QString SUMMARY_BLANK_FIELD = "*None*";
    static inline const QString SUMMARY_UNRESOLVED_FIELD = "*Unresolved*";
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
