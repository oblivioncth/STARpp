#ifndef CALCULATOR_H
#define CALCULATOR_H

// Qt Includes
#include <QObject>

// Project Includes
#include "star/election.h"
#include "star/electionresult.h"

namespace Star
{

class Calculator : public QObject
{
    Q_OBJECT
//-Class Enums------------------------------------------------------------------------------------------------------
public:
    enum ExtendedTiebreakMethod { FiveStar, Condorcet };

//-Class Variables------------------------------------------------------------------------------------------------------
private:


    // Logging - Intro
    static inline const QString LOG_EVENT_INVALID_ELECTION = QStringLiteral("The provided election is invalid.");
    static inline const QString LOG_EVENT_CALC_START = QStringLiteral("Calculating results of election - %1");
    static inline const QString LOG_EVENT_INPUT_COUNTS = QStringLiteral("There are %1 nominees and %2 ballots.");
    static inline const QString LOG_EVENT_INITAL_RAW_RANKINGS = QStringLiteral("Initial score rankings:");

    // Logging - Determine Preliminary Leaders
    static inline const QString LOG_EVENT_DETERMINE_PRELIMINARY_LEADERS = QStringLiteral("Determining preliminary score leaders...");
    static inline const QString LOG_EVENT_PRELIMINARY_FIRST_TIE_BENIGN = QStringLiteral("There is a benign 2-way tie for preliminary first place <%1>:");
    static inline const QString LOG_EVENT_PRELIMINARY_FIRST_TIE = QStringLiteral("There is a %1-way tie for preliminary first place <%2>:");
    static inline const QString LOG_EVENT_PRELIMINARY_FIRST_NO_TIE = QStringLiteral(R"(Preliminary first place is uncontested: "%1")");
    static inline const QString LOG_EVENT_PRELIMINARY_SECOND_TIE = QStringLiteral("There is a %1-way tie for preliminary second place <%2>:");
    static inline const QString LOG_EVENT_PRELIMINARY_SECOND_NO_TIE = QStringLiteral(R"(Preliminary second place is uncontested: "%1")");
    static inline const QString LOG_EVENT_PRELIMINARY_LEADERS = QStringLiteral("Preliminary leaders are:");

    // Logging - Main Runoff
    static inline const QString LOG_EVENT_PERFORM_PRIMARY_RUNOFF = QStringLiteral("Performing primary runoff...");
    static inline const QString LOG_EVENT_PRIMARY_PREF_RANK_SORT = QStringLiteral("Sorting by preference rank to determine winner(s).");
    static inline const QString LOG_EVENT_PRIMARY_FIRST_TIE = QStringLiteral("There is a %1-way tie for primary first place <%2>:");
    static inline const QString LOG_EVENT_PRIMARY_FIRST_NO_TIE = QStringLiteral(R"(Primary first place is uncontested: "%1")");
    static inline const QString LOG_EVENT_PRIMARY_SECOND_TIE = QStringLiteral("There is a %1-way tie for primary second place <%2>:");
    static inline const QString LOG_EVENT_PRIMARY_SECOND_NO_TIE = QStringLiteral(R"(Primary second place is uncontested: "%1")");
    static inline const QString LOG_EVENT_PRIMARY_TIEBREAK_FAIL = QStringLiteral(R"(Primary tiebreak still left a full tie. Taking runner-up from pre-tiebreak second place...)");
    static inline const QString LOG_EVENT_PRIMARY_TIEBREAK_FAIL_NO_FALLBACK = QStringLiteral(R"(No preference second place available.)");

    // Logging - Perform Extended Tiebreak
    static inline const QString LOG_EVENT_EXTENDED_TIEBREAK_EVAL = QStringLiteral("Evaluating extended tiebreak via the %1 method...");
    static inline const QString LOG_EVENT_EXTENDED_TIEBREAK_FAIL = QStringLiteral(R"(Extended tiebreak still left a full tie. Taking runner-up from pre-tiebreak second place...)");
    static inline const QString LOG_EVENT_EXTENDED_TIEBREAK_WINNERS = QStringLiteral(R"(Extended tiebreak winners: { %1 })");
    static inline const QString LOG_EVENT_EXTENDED_TIEBREAK_RUNNERUPS = QStringLiteral(R"(Extended tiebreak runner-ups: { %1 })");

    // Logging - Consider Extended Tiebreak
    static inline const QString LOG_EVENT_EXTENDED_TIEBREAK_SKIP = QStringLiteral("Skipping extended tiebreak consideration as there are no ties after the primary runoff.");
    static inline const QString LOG_EVENT_EXTENDED_TIEBREAK_NO_OP = QStringLiteral("There is a tie after the primary runoff, but neither the extended tiebreak nor tiebreak speculation are enabled.");
    static inline const QString LOG_EVENT_EXTENDED_TIEBREAK_SPECULATIVE = QStringLiteral("Tiebreak speculation is enabled, the results of all extended tiebreak methods will be tested.");
    static inline const QString LOG_EVENT_EXTENDED_TIEBREAK_ENABLED = QStringLiteral(R"(Extended tiebreak with selected method altered outcome, using extended results.)");
    static inline const QString LOG_EVENT_EXTENDED_TIEBREAK_DISABLED = QStringLiteral(R"(Extended tiebreak is not enabled, using original results.)");
    static inline const QString LOG_EVENT_EXTENDED_TIEBREAK_IRRELAVENT = QStringLiteral(R"(Extended tiebreak with selected method resulted in no change.)");

    // Logging - Preference
    static inline const QString LOG_EVENT_RANK_BY_PREF = QStringLiteral("Ranking relevant nominees by preference...");
    static inline const QString LOG_EVENT_RANK_BY_PREF_HAS_PREF = QStringLiteral(R"(%1 prefers "%2" (Total - %3).)");
    static inline const QString LOG_EVENT_RANK_BY_PREF_NO_PREF = QStringLiteral(R"(%1 has no preference.)");
    static inline const QString LOG_EVENT_RANKINGS_PREF = QStringLiteral("Preference Rankings:");

    // Logging - Score
    static inline const QString LOG_EVENT_RANK_BY_SCORE = QStringLiteral("Ranking relevant nominees by score...");
    static inline const QString LOG_EVENT_RANKINGS_SCORE = QStringLiteral("Score Rankings:");

    // Logging - Max Score Votes
    static inline const QString LOG_EVENT_RANK_BY_VOTES_OF_MAX_SCORE = QStringLiteral("Ranking relevant nominees by votes of max score...");
    static inline const QString LOG_EVENT_RANKINGS_VOTES_OF_MAX_SCORE = QStringLiteral("Votes of Max Score Rankings:");

    // Logging - Head-to-head wins
    static inline const QString LOG_EVENT_RANK_BY_HEAD_TO_HEAD_WINS = QStringLiteral("Ranking relevant nominees by head-to-head wins...");
    static inline const QString LOG_EVENT_RANK_BY_HEAD_TO_HEAD_WINS_PREF = QStringLiteral("Using preference ranking to determine winner of %1 vs %2.");
    static inline const QString LOG_EVENT_RANK_BY_HEAD_TO_HEAD_WINS_PREF_WINNER = QStringLiteral("%1 won the head-to-head.");
    static inline const QString LOG_EVENT_RANK_BY_HEAD_TO_HEAD_WINS_PREF_WINNER_IRREL = QStringLiteral("%1 won the head-to-head, but they are not under consideration.");
    static inline const QString LOG_EVENT_RANK_BY_HEAD_TO_HEAD_WINS_PREF_TIE = QStringLiteral("The head to head resulted in a tie, no win assigned to either participant.");
    static inline const QString LOG_EVENT_RANKINGS_HEAD_TO_HEAD_WINS = QStringLiteral("Head-to-head wins Rankings:");

    // Logging - Tiebreak
    static inline const QString LOG_EVENT_BREAK_SCORE_TIE = QStringLiteral("Breaking %1-way score tie...");
    static inline const QString LOG_EVENT_BREAK_PREF_TIE = QStringLiteral("Breaking %1-way preference tie...");
    static inline const QString LOG_EVENT_BREAK_EXTENDED_TIE = QStringLiteral("Breaking %1-way extended tie (%2 method)...");
    static inline const QString LOG_EVENT_BREAK_RESULT = QStringLiteral("Tie Break - First Place { %1 } | Second Place: { %2 }");

    // Logging - Initial Results
    static inline const QString LOG_EVENT_INITIAL_RESULT_WINNERS = QStringLiteral(R"(Initial winners: { %1 })");
    static inline const QString LOG_EVENT_INITIAL_RESULT_RUNNERUPS = QStringLiteral(R"(Initial runner-ups: { %1 })");
    static inline const QString LOG_EVENT_INITIAL_RESULT_WINNER_TIE = QStringLiteral("There is still a tie for first place, checking effect of extended tiebreak...");
    static inline const QString LOG_EVENT_INITIAL_RESULT_RUNNERUP_TIE = QStringLiteral("There is still a tie for second place, checking effect of extended tiebreak...");

    // Logging - Final Results
    static inline const QString LOG_EVENT_FINAL_RESULT_WINNERS = QStringLiteral(R"(Final winners: { %1 })");
    static inline const QString LOG_EVENT_FINAL_RESULT_RUNNERUPS = QStringLiteral(R"(Final runner-ups: { %1 })");

    // Logging - Finish
    static inline const QString LOG_EVENT_CALC_FINISH = QStringLiteral("Calculation complete.");

    // Logging - Lists
    static inline const QString LIST_ITEM_NOMINEE_TOTAL_SCORE = QStringLiteral("\t- \"%1\" <%2>");
    static inline const QString LIST_ITEM_RANK = QStringLiteral("\t%1) { \"%2\" } <%3>");

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    std::optional<ExtendedTiebreakMethod> mExtraTiebreakMethod;
    const Election* mElection;
    bool mSpeculative;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    Calculator(const Election* election = nullptr);

//-Instance Functions-------------------------------------------------------------------------------------------------
private:
    // Main steps
    QSet<QString> determinePreliminaryLeaders();
    QPair<QSet<QString>, QSet<QString>> performPrimaryRunoff(const QSet<QString>& preliminaryLeaders);
    QPair<QSet<QString>, QSet<QString>> performExtendedTiebreak(QSet<QString> initialWinners, QSet<QString> initialRunnerUps, ExtendedTiebreakMethod method);

    // Utility
    QList<Rank> rankByPreference(const QSet<QString>& nominees);
    QList<Rank> rankByScore(const QSet<QString>& nominees);
    QList<Rank> rankByVotesOfMaxScore(const QSet<QString>& nominees);
    QList<Rank> rankByHeadToHeadWins(const QSet<QString>& nominees);

    QPair<QSet<QString>, QSet<QString>> rankBasedTiebreak(const QList<Rank>& rankings, const QString& note);
    QPair<QSet<QString>, QSet<QString>> breakScoreTie(const QSet<QString>& nominees);
    QPair<QSet<QString>, QSet<QString>> breakPreferenceTie(const QSet<QString>& nominees);
    QPair<QSet<QString>, QSet<QString>> breakExtendedTieFiveStar(const QSet<QString>& nominees);
    QPair<QSet<QString>, QSet<QString>> breakExtendedTieCondorcet(const QSet<QString>& nominees);

    // Logging
    QString createNomineeGeneralSetString(const QSet<QString>& nominees);
    QString createNomineeToalScoreSetString(const QSet<QString>& nominees);
    QString createNomineeRankListString(const QList<Rank>& ranks);

public:
    const Election* election() const;
    std::optional<ExtendedTiebreakMethod> extraTiebreakMethod() const;
    bool isExtraTiebreak() const;
    bool isSpeculative() const;

    void setElection(const Election* election);
    void setExtraTiebreakMethod(std::optional<ExtendedTiebreakMethod> method);
    void setSpeculative(bool speculative);

    ElectionResult calculateResult();

//-Signals & Slots-------------------------------------------------------------------------------------------------
signals:
    void calculationDetail(const QString& detail);

};

}

#endif // CALCULATOR_H
