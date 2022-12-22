#ifndef CALCULATOR_H
#define CALCULATOR_H

// Qt Includes
#include <QObject>
#include <QFlags>

// Project Includes
#include "star/election.h"
#include "star/electionresult.h"
#include "star/qualifierresult.h"

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
        AllowTrueTies = 0x01,
        CondorcetProtocol = 0x02
    };
    Q_DECLARE_FLAGS(Options, Option);

//-Class Variables------------------------------------------------------------------------------------------------------
private:
    // Logging - Intro
    static inline const QString LOG_EVENT_INVALID_ELECTION = QStringLiteral("The provided election is invalid.");
    static inline const QString LOG_EVENT_CALC_START = QStringLiteral("Calculating results of election - %1");
    static inline const QString LOG_EVENT_INPUT_COUNTS = QStringLiteral("There are %1 candidates, %2 ballots, and %3 seats to fill.");
    static inline const QString LOG_EVENT_INITAL_RAW_RANKINGS = QStringLiteral("Initial score rankings:");
    static inline const QString LOG_EVENT_CALC_HEAD_TO_HEAD = QStringLiteral("Pre-calculating head-to-head matchup results...");

    // Logging - Perform Runoff Qualifier
    static inline const QString LOG_EVENT_QUALIFIER = QStringLiteral("Performing runoff qualifier to seed candidates for the runoff.");
    static inline const QString LOG_EVENT_QUALIFIER_TOP = QStringLiteral("Trying to determine remaining %1 candidate(s) to advance between:");
    static inline const QString LOG_EVENT_QUALIFIER_ADVANCE_CANDIDATES = QStringLiteral("Advancing candidate(s):");
    static inline const QString LOG_EVENT_QUALIFIER_CUT_CANDIDATES = QStringLiteral("Cutting candidate(s):");
    static inline const QString LOG_EVENT_QUALIFIER_NO_RANDOM = QStringLiteral("Random tiebreaker is disabled.");
    static inline const QString LOG_EVENT_QUALIFIER_UNSUCCESSFUL = QStringLiteral("Unable to resolve scoring round tie to reach the target number of candidates.");
    static inline const QString LOG_EVENT_QUALIFIER_RESULT = QStringLiteral(
        "Qualifier Result:\n"
        "First Seed: %1\n"
        "Second Seed: %2\n"
        "Simultaneous: %3\n"
        "Overflow: {%4}\n"
    );

    // Logging - Determine Scoring round Leaders
    static inline const QString LOG_EVENT_DETERMINE_SCORING_ROUND_LEADERS = QStringLiteral("Determining scoring round score leaders...");
    static inline const QString LOG_EVENT_SCORING_ROUND_FIRST_TIE_BENIGN = QStringLiteral("There is a benign 2-way tie for scoring round first place <%1>:");
    static inline const QString LOG_EVENT_SCORING_ROUND_FIRST_TIE = QStringLiteral("There is a %1-way tie for scoring round first place <%2>:");
    static inline const QString LOG_EVENT_SCORING_ROUND_FIRST_NO_TIE = QStringLiteral(R"(Scoring round first place is uncontested: "%1")");
    static inline const QString LOG_EVENT_SCORING_ROUND_SECOND_TIE = QStringLiteral("There is a %1-way tie for scoring round second place <%2>:");
    static inline const QString LOG_EVENT_SCORING_ROUND_SECOND_NO_TIE = QStringLiteral(R"(Scoring round second place is uncontested: "%1")");
    static inline const QString LOG_EVENT_SCORING_ROUND_LEADERS = QStringLiteral("Scoring round leaders are:");

    // Logging - Main Runoff
    static inline const QString LOG_EVENT_RUNOFF_CANDIDATES = QStringLiteral(R"("%1" & "%2" advance to the runoff.)");
    static inline const QString LOG_EVENT_PERFORM_PRIMARY_RUNOFF = QStringLiteral("Performing primary runoff...");
    static inline const QString LOG_EVENT_RUNOFF_HEAD_TO_HEAD_WINNER_CHECK = QStringLiteral("Checking for clear winner of head-to-head.");
    static inline const QString LOG_EVENT_RUNOFF_TIE = QStringLiteral("The candidates in the runoff are tied in terms of preference.");
    static inline const QString LOG_EVENT_RUNOFF_HIGHER_SCORE_CHECK = QStringLiteral("Checking for the candidate with the higher score.");
    static inline const QString LOG_EVENT_RUNOFF_MORE_FIVE_STAR_CHECK = QStringLiteral("Checking for the candidate with more five star votes.");
    static inline const QString LOG_EVENT_RUNOFF_CHOOSING_RANDOM_WINNER = QStringLiteral("Choosing runoff winner randomly.");
    static inline const QString LOG_EVENT_RUNOFF_NO_RANDOM = QStringLiteral("Random tiebreaker is disabled, the runoff candidates remained tied.");
    static inline const QString LOG_EVENT_RUNOFF_WINNER = QStringLiteral(R"(The runoff resulted in a win for: "%1")");
    static inline const QString LOG_EVENT_RUNOFF_UNRESOLVED = QStringLiteral("The runoff tie could not be broken.");

    // Logging - Ranking
    static inline const QString LOG_EVENT_RANK_BY_SCORE = QStringLiteral("Ranking relevant candidates by score (%1)...");
    static inline const QString LOG_EVENT_RANKINGS_SCORE = QStringLiteral("Score rankings:");
    static inline const QString LOG_EVENT_RANK_BY_VOTES_OF_MAX_SCORE = QStringLiteral("Ranking relevant candidates by votes of max score (%1)...");
    static inline const QString LOG_EVENT_RANKINGS_VOTES_OF_MAX_SCORE = QStringLiteral("Votes of Max Score rankings:");
    static inline const QString LOG_EVENT_RANK_BY_HEAD_TO_HEAD_LOSSES = QStringLiteral("Ranking relevant candidates by head-to-head losses (%1)...");
    static inline const QString LOG_EVENT_RANKINGS_HEAD_TO_HEAD_LOSSES = QStringLiteral("Head-to-head losses rankings:");
    static inline const QString LOG_EVENT_RANK_BY_HEAD_TO_HEAD_PREFERENCES = QStringLiteral("Ranking relevant candidates by head-to-head preference count (%1)...");
    static inline const QString LOG_EVENT_RANKINGS_HEAD_TO_HEAD_PREFERENCES = QStringLiteral("Head-to-head preference count rankings:");
    static inline const QString LOG_EVENT_RANK_BY_HEAD_TO_HEAD_MARGIN = QStringLiteral("Ranking relevant candidates by head-to-head margin (%1)...");
    static inline const QString LOG_EVENT_RANKINGS_HEAD_TO_HEAD_MARGIN = QStringLiteral("Head-to-head margin rankings:");

    // Logging - Tiebreak
    static inline const QString LOG_EVENT_BREAK_TIE_MOST_FIVE_STAR = QStringLiteral("Breaking %1-way tie according to most five star votes...");
    static inline const QString LOG_EVENT_BREAK_TIE_HIGHEST_SCORE = QStringLiteral("Breaking %1-way tie according to highest score...");
    static inline const QString LOG_EVENT_BREAK_TIE_RANDOM = QStringLiteral("Breaking %1-way tie randomly...");
    static inline const QString LOG_EVENT_BREAK_RESULT = QStringLiteral("Tie Break Winner(s) - { %1 }");

    // Logging - Main
    static inline const QString LOG_EVENT_FILLING_SEAT = QStringLiteral("Filling seat %1...");
    static inline const QString LOG_EVENT_DIRECT_SEAT_FILL = QStringLiteral("Only one candidate remains, seat can be filled directly.");
    static inline const QString LOG_EVENT_NO_RUNOFF = QStringLiteral("The number of candidates could not be narrowed to two in order to perform the runoff.");

    // Logging - Final Results
    static inline const QString LOG_EVENT_FINAL_RESULTS = QStringLiteral(
        "Final Results:\n"
        "\n"
        "Filled Seats:\n"
        "%1"
        "\n"
        "Unresolved Candidates:\n"
        "%2"
        "\n"
        "Unfilled Seats: %3\n"
    );

    // Logging - Finish
    static inline const QString LOG_EVENT_CALC_FINISH = QStringLiteral("Calculation complete.");

    // Logging - Lists
    static inline const QString LIST_ITEM_CANDIDATE_TOTAL_SCORE = QStringLiteral("\t- \"%1\" <%2>");
    static inline const QString LIST_ITEM_RANK = QStringLiteral("\t%1) { \"%2\" } <%3>");
    static inline const QString LIST_ITEM_SEAT = QStringLiteral("\t%1) \"%2\"");
    static inline const QString LIST_ITEM_UNRESOLVED = QStringLiteral("\t- \"%1\"");
    static inline const QString LIST_ITEM_NONE = QStringLiteral("\t *NONE*");

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
    QString performPrimaryRunoff(QPair<QString, QString> candidates) const;
    QualifierResult performRunoffQualifier(const QList<Rank>& scoreRankings) const;

    // Utility
    QList<Rank> rankByScore(const QSet<QString>& candidates, Rank::Order order) const;
    QList<Rank> rankByVotesOfMaxScore(const QSet<QString>& candidates, Rank::Order order) const;
    QList<Rank> rankByHeadToHeadLosses(const QSet<QString>& candidates, const HeadToHeadResults* hth, Rank::Order order) const;
    QList<Rank> rankByHeadToHeadPreferences(const QSet<QString>& candidates, const HeadToHeadResults* hth, Rank::Order order) const;
    QList<Rank> rankByHeadToHeadMargin(const QSet<QString>& candidates, const HeadToHeadResults* hth, Rank::Order order) const;

    QSet<QString> rankBasedTiebreak(const QList<Rank>& rankings, const QString& note) const;
    QSet<QString> breakTieMostFiveStar(const QSet<QString>& candidates) const;
    QSet<QString> breakTieHighestScore(const QSet<QString>& candidates) const;
    QString breakTieRandom(const QSet<QString>& candidates) const;

    // Logging
    QString createCandidateGeneralSetString(const QSet<QString>& candidates) const;
    QString createCandidateToalScoreSetString(const QSet<QString>& candidates) const;
    QString createCandidateRankListString(const QList<Rank>& ranks) const;
    void logQualifierResult(const QualifierResult& result) const;
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
