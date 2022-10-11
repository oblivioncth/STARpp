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
    static inline const QString WINNER_TEMPLATE = QStringLiteral(
        "Winner: \"%1\"\n"
        "Runner Up: \"%2\""
    );

    static inline const QString TIE_TEMPLATE = QStringLiteral(
        "Winner (Tie): \"%1\"\n"
        "Winner (Tie): \"%2\""
    );

    static inline const QString RAW_SCORE_TEMPLATE = QStringLiteral(R"("%1" [%2])");

    // Summary
    static inline const QString SUMMARY_WINNER_ITEM = QStringLiteral(R"("%1" (>) "%2")");
    static inline const QString SUMMARY_TIE_ITEM = QStringLiteral(R"("%1" (=) "%2")");

    // Pause
    static inline const QString USER_PAUSE_PROMPT = QStringLiteral("Press ENTER to continue...");

//-Instance Variables------------------------------------------------------------------------------------------------------
private:
    const QList<Star::ElectionResult>* mResults;

//-Constructor----------------------------------------------------------------------------------------------------------
public:
    ResultPresenter(const QList<Star::ElectionResult>* results);

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
