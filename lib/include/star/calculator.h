#ifndef CALCULATOR_H
#define CALCULATOR_H

// Qt Includes
#include <QObject>
#include <QFlags>

// Project Includes
#include "star/election.h"
#include "star/electionresult.h"

namespace Star
{

// Forward Declarations
class HeadToHeadResults;

class Calculator : public QObject
{
    Q_OBJECT
//-Class Enums------------------------------------------------------------------------------------------------------
public:
    enum Option
    {
        NoOptions = 0x00,
        AllowTrueTies = 0x01
    };
    Q_DECLARE_FLAGS(Options, Option);

//-Class Variables------------------------------------------------------------------------------------------------------
private:
    // Logging - Intro
    static inline const QString LOG_EVENT_INVALID_ELECTION = QStringLiteral("The provided election is invalid.");
    static inline const QString LOG_EVENT_CALC_START = QStringLiteral("Calculating results of election - %1");
    static inline const QString LOG_EVENT_INPUT_COUNTS = QStringLiteral("There are %1 nominees, %2 ballots, and %3 seats to fill.");
    static inline const QString LOG_EVENT_INITAL_RAW_RANKINGS = QStringLiteral("Initial score rankings:");
    static inline const QString LOG_EVENT_CALC_HEAD_TO_HEAD = QStringLiteral("Pre-calculating head-to-head matchup results...");

    // Logging - Determine Preliminary Leaders
    static inline const QString LOG_EVENT_DETERMINE_PRELIMINARY_LEADERS = QStringLiteral("Determining preliminary score leaders...");
    static inline const QString LOG_EVENT_PRELIMINARY_FIRST_TIE_BENIGN = QStringLiteral("There is a benign 2-way tie for preliminary first place <%1>:");
    static inline const QString LOG_EVENT_PRELIMINARY_FIRST_TIE = QStringLiteral("There is a %1-way tie for preliminary first place <%2>:");
    static inline const QString LOG_EVENT_PRELIMINARY_FIRST_NO_TIE = QStringLiteral(R"(Preliminary first place is uncontested: "%1")");
    static inline const QString LOG_EVENT_PRELIMINARY_SECOND_TIE = QStringLiteral("There is a %1-way tie for preliminary second place <%2>:");
    static inline const QString LOG_EVENT_PRELIMINARY_SECOND_NO_TIE = QStringLiteral(R"(Preliminary second place is uncontested: "%1")");
    static inline const QString LOG_EVENT_PRELIMINARY_LEADERS = QStringLiteral("Preliminary leaders are:");

    // Logging - Preliminary Candidate Tie Reduction
    static inline const QString LOG_EVENT_PRELIMINARY_TIE_REDUCTION = QStringLiteral("Resolving %1-way tie down to the target of %2 candidates...");
    static inline const QString LOG_EVENT_PPRELIMINARY_TIE_CUT_CANDIDATE = QStringLiteral(R"(Cutting candidate "%1")");
    static inline const QString LOG_EVENT_PRELIMINARY_NO_RANDOM = QStringLiteral("Random tiebreaker is disabled.");
    static inline const QString LOG_EVENT_PPRELIMINARY_TIE_REDUCTION_UNSUCCESSFUL = QStringLiteral(R"(Unable to resolve preliminary tie to reach the target number of candidates.)");
    static inline const QString LOG_EVENT_PPRELIMINARY_TIE_REDUCTION_RESULT = QStringLiteral("Preliminary tie reduced to:");

    // Logging - Main Runoff
    static inline const QString LOG_EVENT_RUNOFF_CANDIDATES = QStringLiteral(R"("%1" & "%2" advance to the runoff.)");
    static inline const QString LOG_EVENT_PERFORM_PRIMARY_RUNOFF = QStringLiteral("Performing primary runoff...");
    static inline const QString LOG_EVENT_PRIMARY_HEAD_TO_HEAD_WINNER_CHECK = QStringLiteral("Checking for clear winner of head-to-head.");
    static inline const QString LOG_EVENT_PRIMARY_TIE = QStringLiteral("There candidates in the runoff are tied in terms of preference.");
    static inline const QString LOG_EVENT_PRIMARY_HIGHER_SCORE_CHECK = QStringLiteral("Checking for the candidate with the higher score.");
    static inline const QString LOG_EVENT_PRIMARY_MORE_FIVE_STAR_CHECK = QStringLiteral("Checking for the candidate with more five star votes.");
    static inline const QString LOG_EVENT_PRIMARY_CHOOSING_RANDOM_WINNER = QStringLiteral("Choosing runoff winner randomly.");
    static inline const QString LOG_EVENT_PRIMARY_NO_RANDOM = QStringLiteral("Random tiebreaker is disabled, the runoff candidates remained tied.");
   // static inline const QString LOG_EVENT_CONDORCET_START_STAGES = QStringLiteral("Following STAR Condorcet protocol methodology...");
    static inline const QString LOG_EVENT_PRIMARY_WINNER = QStringLiteral(R"(The runoff resulted in a win for: "%1")");
    static inline const QString LOG_EVENT_PRIMARY_UNRESOLVED = QStringLiteral(R"(The runoff tie could not be broken.)");

    // Logging - Ranking
    static inline const QString LOG_EVENT_RANK_BY_SCORE = QStringLiteral("Ranking relevant nominees by score (%1)...");
    static inline const QString LOG_EVENT_RANKINGS_SCORE = QStringLiteral("Score rankings:");
    static inline const QString LOG_EVENT_RANK_BY_VOTES_OF_MAX_SCORE = QStringLiteral("Ranking relevant nominees by votes of max score (%1)...");
    static inline const QString LOG_EVENT_RANKINGS_VOTES_OF_MAX_SCORE = QStringLiteral("Votes of Max Score rankings:");
    static inline const QString LOG_EVENT_RANK_BY_HEAD_TO_HEAD_LOSSES = QStringLiteral("Ranking relevant nominees by head-to-head losses (%1)...");
    static inline const QString LOG_EVENT_RANKINGS_HEAD_TO_HEAD_LOSSES = QStringLiteral("Head-to-head losses rankings:");
    static inline const QString LOG_EVENT_RANK_BY_HEAD_TO_HEAD_PREFERENCES = QStringLiteral("Ranking relevant nominees by head-to-head preference count (%1)...");
    static inline const QString LOG_EVENT_RANKINGS_HEAD_TO_HEAD_PREFERENCES = QStringLiteral("Head-to-head preference count rankings:");
    static inline const QString LOG_EVENT_RANK_BY_HEAD_TO_HEAD_MARGIN = QStringLiteral("Ranking relevant nominees by head-to-head margin (%1)...");
    static inline const QString LOG_EVENT_RANKINGS_HEAD_TO_HEAD_MARGIN = QStringLiteral("Head-to-head margin rankings:");

    // Logging - Tiebreak
    static inline const QString LOG_EVENT_BREAK_TIE_MOST_FIVE_STAR = QStringLiteral("Breaking %1-way tie according to most five star votes...");
    static inline const QString LOG_EVENT_BREAK_TIE_LEAST_FIVE_STAR = QStringLiteral("Breaking %1-way tie according to most least star votes...");
    static inline const QString LOG_EVENT_BREAK_TIE_MOST_HEAD_TO_HEAD_LOSSES = QStringLiteral("Breaking %1-way tie according to most head-to-head losses...");
    static inline const QString LOG_EVENT_BREAK_TIE_LEAST_HEAD_TO_HEAD_PREFERENCES = QStringLiteral("Breaking %1-way tie according to least head-to-head preferences...");
    static inline const QString LOG_EVENT_BREAK_TIE_SMALLEST_HEAD_TO_HEAD_MARGIN = QStringLiteral("Breaking %1-way tie according to smallest head-to-head margin...");
    static inline const QString LOG_EVENT_BREAK_TIE_HIGHEST_SCORE = QStringLiteral("Breaking %1-way tie according to highest score...");
    static inline const QString LOG_EVENT_BREAK_TIE_RANDOM = QStringLiteral("Breaking %1-way tie randomly...");
    static inline const QString LOG_EVENT_BREAK_RESULT = QStringLiteral("Tie Break Winner(s) - { %1 }");

    // Logging - Main
    static inline const QString LOG_EVENT_FILLING_SEAT = QStringLiteral("Filling seat %1...");
    static inline const QString LOG_EVENT_NO_RUNOFF = QStringLiteral("The number of candidates could not be narrowed to two in order to perform the runoff.");

    // Logging - Final Results
    static inline const QString LOG_EVENT_FINAL_RESULTS = QStringLiteral("Final Results:\n"
                                                                         "--------------\n"
                                                                         "\n"
                                                                         "Filled Seats:\n"
                                                                         "%1"
                                                                         "\n"
                                                                         "Unresolved Candidates:\n"
                                                                         "%2"
                                                                         "\n"
                                                                         "Unfilled Seats: %3\n");

    // Logging - Finish
    static inline const QString LOG_EVENT_CALC_FINISH = QStringLiteral("Calculation complete.");

    // Logging - Lists
    static inline const QString LIST_ITEM_NOMINEE_TOTAL_SCORE = QStringLiteral("\t- \"%1\" <%2>");
    static inline const QString LIST_ITEM_RANK = QStringLiteral("\t%1) { \"%2\" } <%3>");
    static inline const QString LIST_ITEM_SEAT = QStringLiteral("\t%1) \"%2\"");
    static inline const QString LIST_ITEM_UNRESOLVED = QStringLiteral("\t- \"%1\"");

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    const Election* mElection;
    std::unique_ptr<HeadToHeadResults> mHeadToHeadResults;
    Options mOptions;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    Calculator(const Election* election = nullptr);

//-Destructor---------------------------------------------------------------------------------------------------------
public:
    /* Required for std::unique_ptr<HeadToHeadResults> member to work correctly
     *
     * See: https://stackoverflow.com/questions/33212686/how-to-use-unique-ptr-with-forward-declared-type
     */
    ~Calculator();

//-Instance Functions-------------------------------------------------------------------------------------------------
private:
    // Main steps
    QSet<QString> determinePreliminaryLeaders(const QList<Rank>& scoreRankings);
    QString performPrimaryRunoff(QPair<QString, QString> candidates) const;
    QSet<QString> preliminaryCandidateTieReduction(QSet<QString> candidates, qsizetype desiredCount) const;

    // Utility
    QList<Rank> rankByScore(const QSet<QString>& nominees, Rank::Order order) const;
    QList<Rank> rankByVotesOfMaxScore(const QSet<QString>& nominees, Rank::Order order) const;
    QList<Rank> rankByHeadToHeadLosses(const QSet<QString>& nominees, const HeadToHeadResults* hth, Rank::Order order) const;
    QList<Rank> rankByHeadToHeadPreferences(const QSet<QString>& nominees, const HeadToHeadResults* hth, Rank::Order order) const;
    QList<Rank> rankByHeadToHeadMargin(const QSet<QString>& nominees, const HeadToHeadResults* hth, Rank::Order order) const;

    QSet<QString> rankBasedTiebreak(const QList<Rank>& rankings, const QString& note) const;
    QSet<QString> breakTieMostFiveStar(const QSet<QString>& nominees) const;
    QSet<QString> breakTieLeastFiveStar(const QSet<QString>& nominees) const;
    QSet<QString> breakTieMostHeadToHeadLosses(const QSet<QString>& nominees, const HeadToHeadResults* hth) const;
    QSet<QString> breakTieLeastHeadToHeadPreferences(const QSet<QString>& nominees, const HeadToHeadResults* hth) const;
    QSet<QString> breakTieSmallestHeadToHeadMargin(const QSet<QString>& nominees, const HeadToHeadResults* hth) const;
    QSet<QString> breakTieHighestScore(const QSet<QString>& nominees) const;
    //QSet<QString> breakTieCondorcetProtocol(const QSet<QString>& nominees, const HeadToHeadResults* hth);
    QString breakTieRandom(const QSet<QString>& nominees) const;

    // Logging
    QString createNomineeGeneralSetString(const QSet<QString>& nominees) const;
    QString createNomineeToalScoreSetString(const QSet<QString>& nominees) const;
    QString createNomineeRankListString(const QList<Rank>& ranks) const;
    void logElectionResults(const ElectionResult& results) const;

public:
    const Election* election() const;
    Options options() const;

    void setElection(const Election* election);
    void setOptions(Options options);

    ElectionResult calculateResult();

//-Signals & Slots-------------------------------------------------------------------------------------------------
signals:
    void calculationDetail(const QString& detail) const; // clazy:exclude=const-signal-or-slot
};
Q_DECLARE_OPERATORS_FOR_FLAGS(Calculator::Options)

}

#endif // CALCULATOR_H
