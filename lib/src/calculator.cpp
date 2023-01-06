// Unit Include
#include "star/calculator.h"

// Qt Includes
#include <QRandomGenerator>

// Qx Includes
#include <qx/core/qx-string.h>
#include <qx/core/qx-algorithm.h>

// magic_enum Includes
#include <magic_enum.hpp>

// Project Includes
#include "headtoheadresults.h"

//-Macros----------------------------------------
#define ENUM_NAME(eenum) QString(magic_enum::enum_name(eenum).data())

namespace Star
{

//===============================================================================================================
// Calculator
//===============================================================================================================

/*!
 *  @class Calculator star/calculator.h
 *
 *  @brief The Calculator class provides the means to determine the result of an election.
 *
 *  A Calculator acts as the actual apparatus through which election results are computed. Once an Election has
 *  been prepared, its outcome can be realized by constructing a Calculator instance for it via
 *  Calculator(const Election*), followed by calling calculateResult().
 *
 *  If running multiple elections, there is no need to create a Calculator for each one, as a single Calculator
 *  can be reused for subsequent elections by using setElection().
 *
 *  @note An ElectionResult created by calculateResult() requires referring to the Election it originated from
 *  in order to return the correct values from some of its methods; therefore, generally one should guarantee
 *  that Elections provided to a Calculator will not be deleted as long as their corresponding results are in
 *  use.
 *
 *  @par Options
 *  @parblock
 *  A Calculator features several configurable options that alter some aspects of winner determination. They
 *  can be set through setOptions() and queried by options(). Some of these options are directly sanctioned
 *  by the STAR voting project, while others are specific to this library; however, STAR voting ultimately
 *  allows alteration of fine election details as long as all parties involved agree upon them before an
 *  election is run.
 *
 *  @c NoOptions
 *  This is the default and leads Calculator to elect candidates by exactly following standard STAR
 *  procedure.
 *
 *  @c AllowTrueTies
 *  By default, the STAR election procedure will resort to breaking ties randomly if all other tiebreak
 *  procedures have been exhausted and a tie still remains. If this isn't acceptable for your particular
 *  purposes, this option instead leaves the current seat unfilled and ends the election prematurely if
 *  this situation occurs.
 *
 *  While not standard, this option is recognized by the STAR project.
 *
 *  @c CondorcetProtocol
 *  This adds the "Condorcet Protocol" as a fallback tiebreaker that is used before resorting to a random
 *  tiebreaker during a tie. First, this attempts to break the tie by checking which candidate had highest
 *  preference count (preferred the most times total) among all their head-to-head matchups. Then, if that
 *  fails, it tries to break the tie by checking which candidate had the greatest preference margin (their
 *  total preference count minus the total number of times they were preferred against).
 *
 *  While not standard, and also not recommended due to the added complexity, this option is recognized
 *  by the STAR project.
 *
 *  @c DefactoWinner
 *  This option was specifically created to solve a specific issue that is more likely to occur with smaller
 *  elections when AllowTrueTies is also turned on.
 *
 *  It it quite common that the first candidate seeded into the runoff for a given seat will end up winning
 *  that seat; however, if an unresolvable tie for second seed occurs then the runoff never happens and so
 *  that fact ends up being irrelevant. But because of that same fact, if a runoff was evaluated by hand between
 *  the first seed and each of the remainder stuck battling for second seed, one will find that much of the
 *  time the first seed wins against all of them.
 *
 *  Ending the election prematurely in this circumstance would be unfortunate, since it is essentially known
 *  that the first seed should would the seat anyway.
 *
 *  Enabling this option avoids the issue by simulating the runoffs between the first seed and each of the
 *  candidates tied for second seed. If the first does win all of the simulations, then they are declared
 *  the winner of that seat.
 *
 *  This option is completely non-standard and not recognized by the STAR project.
 *  @endparblock
 *
 *  @sa Election.
 */

//-Class Enums-----------------------------------------------------------------------------------------------
//Public:
/*!
 *  @enum Calculator::Option
 *
 *  This enum represents the various rules that can be applied to a Calculator when determining the results
 *  of an Election.
 */

/*!
 *  @var Calculator::Option Calculator::NoOptions
 *  Use the default STAR determination procedure.
 */

/*!
 *  @var Calculator::Option Calculator::AllowTrueTies
 *  Ends an election prematurely instead of using a random tiebreaker when an unresolvable tie occurs.
 */

/*!
 *  @var Calculator::Option Calculator::CondorcetProtocol
 *  Uses the Condorcet protocol during the scoring round before the random tiebreaker if necessary.
 */

/*!
 *  @var Calculator::Option Calculator::DefactoWinner
 *  If an unresolvable tie occurs for second seed in the qualifier, gives the win to the first seed if
 *  they would defeat each tied candidate independently in the runoff. Has no effect if AllowTrueTies is
 *  not enabled.
 */

/*!
 *  @qflag{Calculator::Options, Calculator::Option}
 */

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
/*!
 * Creates a calculator set to evaluate the Election @a election.
 *
 * @sa setElection().
 */
Calculator::Calculator(const Election* election) :
    mElection(election),
    mOptions(Option::NoOptions)
{}

/*!
 * Destroys the calculator
 */
Calculator::~Calculator() = default;

//-Instance Functions-------------------------------------------------------------------------------------------------
//Private:
QualifierResult Calculator::performRunoffQualifier(const QList<Rank>& scoreRankings) const
{
    /* Overall this function attempts to break the tied candidates by selecting the winner(s) of the tie in
     * a similar fashion to Bloc voting. One candidate is selected as the winner and then if a second
     * candidate is needed (i.e. tie for first place score) the process is repeated with the first winner
     * removed. There is an exception/shortcut during the 5-star tiebreaker step however, in which more than
     * one candidate can be set aside.
     */

    emit calculationDetail(LOG_EVENT_QUALIFIER);

    // Only the first two score ranks are ever relevant
    QList<Rank> contenderRankings = scoreRankings.first(std::min(scoreRankings.size(), qsizetype(2)));

    // Results to fill
    QSet<QString> firstAdvancement;
    QSet<QString> secondAdvancement;

    // Need tracker
    qsizetype candidatesNeeded = 2;

    // Convenience functions for manipulating candidates
    const auto removeContenders = [&](const QSet<QString>& c){
        Rank& topRank = contenderRankings.first();
        topRank.candidates.subtract(c);
        if(topRank.candidates.isEmpty())
            contenderRankings.removeFirst();
    };

    const auto advanceCandidates = [&](const QSet<QString>& c){
        emit calculationDetail(LOG_EVENT_QUALIFIER_ADVANCE_CANDIDATES + '\n' + createCandidateGeneralSetString(c));

        if(firstAdvancement.isEmpty())
            firstAdvancement = c;
        else if(secondAdvancement.isEmpty())
            secondAdvancement = c;
        else
            qFatal("attempted to advance candidates more than twice!");

        removeContenders(c);
        candidatesNeeded -= c.size();
    };

    // Seed candidates until at 2
    while(candidatesNeeded > 0)
    {
        // Get candidates from top score rank
        QSet<QString> topCandidates = contenderRankings.front().candidates;
        emit calculationDetail(LOG_EVENT_QUALIFIER_TOP.arg(candidatesNeeded) + '\n' + createCandidateGeneralSetString(topCandidates));

        // Advance candidates directly if possible
        if(topCandidates.size() <= candidatesNeeded)
        {
            advanceCandidates(topCandidates);
            continue;
        }

        // Get head-to-head results of score tied candidates
        HeadToHeadResults tiedHtH = mHeadToHeadResults->narrowed(topCandidates, HeadToHeadResults::Inclusive);

        // Convenience functions for candidate culling and advancement
        const auto tryCullLosers = [&](const QList<Rank>& loserFirstRankings){
            if(loserFirstRankings.size() > 1)
            {
                const QSet<QString>& toCut = loserFirstRankings.front().candidates;
                emit calculationDetail(LOG_EVENT_QUALIFIER_CUT_CANDIDATES + '\n' + createCandidateGeneralSetString(toCut));
                tiedHtH.narrow(toCut, HeadToHeadResults::Exclusive);
                return true;
            }
            else
                return false;
        };

        const auto tryAdvanceRemaining = [&]{
            if(tiedHtH.candidateCount() <= candidatesNeeded)
            {
                advanceCandidates(tiedHtH.candidates());
                return true;
            }
            else
                return false;
        };

        // First reduce candidates by removing those with the most head-to-head losses until there is an across the board tie
        forever
        {
            // Sort remaining by head-to-head losses
            const QList<Rank> lossRankings = rankByHeadToHeadLosses(tiedHtH.candidates(), &tiedHtH, Rank::Descending);

            // If possible, remove the candidates with the most losses, adjust the head-to-head results, then repeat
            if(tryCullLosers(lossRankings))
                continue;

            // If the remaining candidates have been reduced below the required amount advance them, then restart whole process
            if(tryAdvanceRemaining())
                break;

            // Sort remaining by 5-star votes
            const QList<Rank> fiveStarRankings = rankByVotesOfMaxScore(tiedHtH.candidates(), Rank::Ascending);

            /* If possible, advance the clear 5-star winner(s), then restart the whole process
             *
             * In this exception case, the intent is to take as many candidates from the lead (bottom here) backwards
             * as possible in order to populate the advancing candidates, unlike other steps where only the top-most/bottom-most
             * rank is considered. This is essentially just to cover this specific situation:
             *
             * 5-Star Ranks (Ascending)
             * ------------------------
             * 1) Adam, Jen (0)
             * 2) Kyle (3)
             * 3) Mandy (5)
             *
             * Here if two candidates are still needed, Mandy and Kyle would be set aside for advancement at the same time instead of
             * just taking Mandy and then restarting.
             */
            QSet<QString> fiveStarAdv;

            /* TODO: for(const Rank& r : fiveStarRankings | std::views::reverse)
             *
             * Ranges are broken in clang and the fix likely won't be available until clang16 (long time till that becomes standard)
             *
             * https://github.com/llvm/llvm-project/issues/44178
             */
            for(auto rItr = fiveStarRankings.crbegin(); rItr != fiveStarRankings.crend(); rItr++)
            {
                qsizetype room = candidatesNeeded - fiveStarAdv.size();

                if(room == 0)
                    break;

                if(rItr->candidates.size() <= room)
                    fiveStarAdv.unite(rItr->candidates);
                else
                    break;
            }

            if(!fiveStarAdv.isEmpty())
            {
                advanceCandidates(fiveStarAdv);
                break;
            }

            // If there are clear 5-star losers, remove them and then repeat this sub process
            if(tryCullLosers(fiveStarRankings))
                continue;

            // Handle Condorcet protocol specific steps if enabled
            if(mOptions.testFlag(Option::CondorcetProtocol))
            {
                /* These steps are not officially part of the new tiebreak procedure, but they've been adapted to fit
                 * the rest of the process as best as possible
                 */

                // Sort remaining by head-to-head preferences
                const QList<Rank> prefRankings = rankByHeadToHeadPreferences(tiedHtH.candidates(), &tiedHtH, Rank::Ascending);

                // If possible, remove the candidates with the least preferences, adjust the head-to-head results, then repeat this sub process
                if(tryCullLosers(prefRankings))
                    continue;

                // If the remaining candidates have been reduced below the required amount advance them, then restart whole process
                if(tryAdvanceRemaining())
                    break;

                // Sort remaining by head-to-head margin
                const QList<Rank> marginRankings = rankByHeadToHeadMargin(tiedHtH.candidates(), &tiedHtH, Rank::Ascending);

                // If possible, remove the candidates with the lowest margin, adjust the head-to-head results, then repeat this sub process
                if(tryCullLosers(marginRankings))
                    continue;

                // If the remaining candidates have been reduced below the required amount advance them, then restart whole process
                if(tryAdvanceRemaining())
                    break;
            }

            // If true ties are not allowed, use a random tiebreak to select one to advance; otherwise, simply "advance" the remaining candidates
            if(!mOptions.testFlag(Option::AllowTrueTies))
                advanceCandidates({breakTieRandom(tiedHtH.candidates())});
            else
            {
                emit calculationDetail(LOG_EVENT_QUALIFIER_NO_RANDOM);
                advanceCandidates(tiedHtH.candidates());
            }

            break;
        }
    }

    // Create qualifier result
    QualifierResult res(firstAdvancement, secondAdvancement);

    // Note if a true tie occurred
    if(!res.isComplete())
        emit calculationDetail(LOG_EVENT_QUALIFIER_UNSUCCESSFUL);

    // Return the qualifier result set, ideally complete
    logQualifierResult(res);
    return res;
}

bool Calculator::checkForDefactoWinner(const QString& firstSeed, const QSet<QString>& overflow) const
{
    emit calculationDetail(LOG_EVENT_DEFACTO_WINNER_CHECK.arg(firstSeed) + '\n' + createCandidateGeneralSetString(overflow));

    for(const QString& other : overflow)
    {
        std::pair<QString, QString> matchup = std::make_pair(firstSeed, other);

        if(performRunoff(matchup) == firstSeed)
            emit calculationDetail(LOG_EVENT_DEFACTO_WINNER_CHECK_WIN.arg(other));
        else
        {
            emit calculationDetail(LOG_EVENT_DEFACTO_WINNER_CHECK_FAIL.arg(other));
            return false;
        }
    }

    emit calculationDetail(LOG_EVENT_DEFACTO_WINNER_CHECK_SUCCESS);
    return true;
}

QString Calculator::performRunoff(std::pair<QString, QString> candidates) const
{
    emit calculationDetail(LOG_EVENT_RUNOFF.arg(candidates.first, candidates.second));

    // Check for clear winner
    emit calculationDetail(LOG_EVENT_RUNOFF_HEAD_TO_HEAD_WINNER_CHECK);
    QString winner = mHeadToHeadResults->winner(candidates.first, candidates.second);
    if(winner.isNull())
    {
        emit calculationDetail(LOG_EVENT_RUNOFF_TIE);
        QSet<QString> cTied = {candidates.first, candidates.second};

        // Try to break tie by original score
        emit calculationDetail(LOG_EVENT_RUNOFF_HIGHER_SCORE_CHECK);
        QSet<QString> highestScore = breakTieHighestScore(cTied);
        if(highestScore.size() == 1)
            winner = *highestScore.cbegin();
        else
        {
            // Try to break tie by five star votes
            emit calculationDetail(LOG_EVENT_RUNOFF_MORE_FIVE_STAR_CHECK);
            QSet<QString> mostFiveStar = breakTieMostFiveStar(cTied);
            if(mostFiveStar.size() == 1)
                winner = *mostFiveStar.cbegin();
            else
            {
                // Randomly choose a winner if allowed
                if(!mOptions.testFlag(Option::AllowTrueTies))
                {
                    emit calculationDetail(LOG_EVENT_RUNOFF_CHOOSING_RANDOM_WINNER);
                    winner = breakTieRandom(cTied);
                }
                else
                    emit calculationDetail(LOG_EVENT_RUNOFF_NO_RANDOM);
            }
        }
    }

    // Note results
    emit calculationDetail(winner.isNull() ? LOG_EVENT_RUNOFF_UNRESOLVED : LOG_EVENT_RUNOFF_WINNER.arg(winner));

    // Return result
    return winner;
}

QList<Rank> Calculator::rankByScore(const QSet<QString>& candidates, Rank::Order order) const
{
    /* Determine aggregate score of candidate list
     * Redoing this with the provided sub-list is more straight forward than trying to manipulate
     * the full score rankings that are part of the Election
     */
    emit calculationDetail(LOG_EVENT_RANK_BY_SCORE.arg(ENUM_NAME(order)));
    QMap<QString, int> totalScoreMap;

    for(const QString& candidate : candidates)
        totalScoreMap[candidate] = mElection->totalScore(candidate);

    // Create sorted rank list
    QList<Rank> scoreRanks = Rank::rankSort(totalScoreMap, order);

    emit calculationDetail(LOG_EVENT_RANKINGS_SCORE + '\n' + createCandidateRankListString(scoreRanks));
    return scoreRanks;
}

QList<Rank> Calculator::rankByVotesOfMaxScore(const QSet<QString>& candidates, Rank::Order order) const
{
    // Determine aggregate max votes of candidate list
    emit calculationDetail(LOG_EVENT_RANK_BY_VOTES_OF_MAX_SCORE.arg(ENUM_NAME(order)));
    QMap<QString, int> totalMaxVotesMap;

    for(const QString& candidate : candidates)
    {
        uint maxVoteCount = 0;

        for(const Star::Election::Ballot& ballot : mElection->ballots())
            if(ballot.score(candidate) == 5)
                maxVoteCount++;

        totalMaxVotesMap[candidate] = maxVoteCount;
    }

    // Create sorted rank list
    QList<Rank> maxVoteRanks = Rank::rankSort(totalMaxVotesMap, order);

    emit calculationDetail(LOG_EVENT_RANKINGS_VOTES_OF_MAX_SCORE + '\n' + createCandidateRankListString(maxVoteRanks));
    return maxVoteRanks;
}

QList<Rank> Calculator::rankByHeadToHeadLosses(const QSet<QString>& candidates, const HeadToHeadResults* hth, Rank::Order order) const
{
    // Create losses map
    emit calculationDetail(LOG_EVENT_RANK_BY_HEAD_TO_HEAD_LOSSES.arg(ENUM_NAME(order)));
    QMap<QString, int> losses;

    for(const QString& c : candidates)
        losses[c] = hth->losses(c);

    // Create sorted wins losses list
    QList<Rank> headToHeadLossesRanks = Rank::rankSort(losses, order);

    emit calculationDetail(LOG_EVENT_RANKINGS_HEAD_TO_HEAD_LOSSES + '\n' + createCandidateRankListString(headToHeadLossesRanks));
    return headToHeadLossesRanks;
}

QList<Rank> Calculator::rankByHeadToHeadPreferences(const QSet<QString>& candidates, const HeadToHeadResults* hth, Rank::Order order) const
{
    // Determine aggregate face-off wins of candidates list
    emit calculationDetail(LOG_EVENT_RANK_BY_HEAD_TO_HEAD_PREFERENCES.arg(ENUM_NAME(order)));

    // Create pref count map
    QMap<QString, int> preferences;
    for(const QString& c : candidates)
        preferences[c] = hth->preferences(c);

    // Create scoped & sorted wins list
    QList<Rank> headToHeadPrefCountRanks = Rank::rankSort(preferences, order);

    emit calculationDetail(LOG_EVENT_RANKINGS_HEAD_TO_HEAD_PREFERENCES + '\n' + createCandidateRankListString(headToHeadPrefCountRanks));
    return headToHeadPrefCountRanks;
}

QList<Rank> Calculator::rankByHeadToHeadMargin(const QSet<QString>& candidates, const HeadToHeadResults* hth, Rank::Order order) const
{
    // Determine aggregate face-off wins of candidates list
    emit calculationDetail(LOG_EVENT_RANK_BY_HEAD_TO_HEAD_MARGIN.arg(ENUM_NAME(order)));

    // Create pref count map
    QMap<QString, int> margins;
    for(const QString& c : candidates)
        margins[c] = hth->margin(c);

    // Create scoped & sorted wins list
    QList<Rank> headToHeadMarginRanks = Rank::rankSort(margins, order);

    emit calculationDetail(LOG_EVENT_RANKINGS_HEAD_TO_HEAD_MARGIN + '\n' + createCandidateRankListString(headToHeadMarginRanks));
    return headToHeadMarginRanks;
}

QSet<QString> Calculator::rankBasedTiebreak(const QList<Rank>& rankings, const QString& note) const
{
    // Break a tie by using the provided rankings
    emit calculationDetail(note);
    QSet<QString> tieBreak = rankings.front().candidates;

    emit calculationDetail(LOG_EVENT_BREAK_RESULT.arg(Qx::String::join(tieBreak, ", ")));
    return tieBreak;
}

QSet<QString> Calculator::breakTieMostFiveStar(const QSet<QString>& candidates) const
{
    return rankBasedTiebreak(rankByVotesOfMaxScore(candidates, Rank::Descending), LOG_EVENT_BREAK_TIE_MOST_FIVE_STAR.arg(candidates.size()));
}

QSet<QString> Calculator::breakTieHighestScore(const QSet<QString>& candidates) const
{
    return rankBasedTiebreak(rankByScore(candidates, Rank::Descending), LOG_EVENT_BREAK_TIE_HIGHEST_SCORE.arg(candidates.size()));
}

QString Calculator::breakTieRandom(const QSet<QString>& candidates) const
{
    emit calculationDetail(LOG_EVENT_BREAK_TIE_RANDOM.arg(candidates.size()));

    /* Randomly select a winner/loser of the tiebreak
     *
     * There is already some level of randomness since the iteration order of a set is undefined, but this is not enough on its own.
     */
    quint32 selection = QRandomGenerator::global()->bounded(candidates.size());

    auto itr = candidates.constBegin();
    for(qsizetype i = 0; i < selection; i++)
        itr ++;

    return *itr;
}

QString Calculator::createCandidateGeneralSetString(const QSet<QString>& candidates) const
{
    if(candidates.isEmpty())
        return QString();

    return Qx::String::join(candidates, "\n", "\t- ");
}

QString Calculator::createCandidateToalScoreSetString(const QSet<QString>& candidates) const
{
    if(candidates.isEmpty())
        return QString();

    QString listStr;

    auto itr = candidates.constBegin();
    while(itr != candidates.constEnd())
    {
        const QString& candidate = *itr;
        QString candidateStr = LIST_ITEM_CANDIDATE_TOTAL_SCORE.arg(candidate).arg(mElection->totalScore(candidate));

        if(++itr != candidates.constEnd())
            candidateStr += '\n';
        listStr.append(candidateStr);
    }

    return listStr;
}

QString Calculator::createCandidateRankListString(const QList<Rank>& ranks) const
{
    if(ranks.isEmpty())
        return QString();

    QString listStr;
    qsizetype lastIdx = ranks.size() - 1;
    for(qsizetype i = 0; i < ranks.size(); i++)
    {
        const Rank& rank = ranks.at(i);
        QString candidateStr = LIST_ITEM_RANK.arg(i).arg(Qx::String::join(rank.candidates, R"(", ")")).arg(rank.value);
        if(i != lastIdx)
            candidateStr += '\n';
        listStr.append(candidateStr);
    }

    return listStr;
}

void Calculator::logQualifierResult(const QualifierResult& result) const
{
    QString strFs = !result.hasFirstSeed() ? "" : '"' + result.firstSeed() + '"';
    QString strSs = !result.hasSecondSeed() ? "" : '"' + result.secondSeed() + '"';
    QString strOf = result.isComplete() ? "" : '"' + Qx::String::join(result.overflow(), R"(", ")") + '"';

    // Log
    emit calculationDetail(LOG_EVENT_QUALIFIER_RESULT.arg(strFs, strSs, result.isSeededSimultaneously() ? "true" : "false", strOf));
}

void Calculator::logElectionResults(const ElectionResult& results) const
{
    // Create filled seats string
    QString seatListStr;
    const QStringList cWinners = results.winners();

    if(cWinners.isEmpty())
        seatListStr = LIST_ITEM_NONE + '\n';
    else
    {
        for(qsizetype i = 0; i < results.winners().size(); i++)
        {
            QString seatWinner = results.winners().at(i);
            QString seatStr = LIST_ITEM_SEAT.arg(i).arg(seatWinner);
            seatStr += '\n';
            seatListStr.append(seatStr);
        }
    }

    // Create unresolved candidates string
    QString unresolvedListStr;
    const QSet<QString> cUnresolved = results.unresolvedCandidates();

    if(cUnresolved.isEmpty())
        unresolvedListStr = LIST_ITEM_NONE + '\n';
    else
    {
        for(auto itr = cUnresolved.cbegin(); itr != cUnresolved.cend(); itr++)
        {
            QString candidate = *itr;
            QString unresolvedStr = LIST_ITEM_UNRESOLVED.arg(candidate);
            unresolvedStr += '\n';
            unresolvedListStr.append(unresolvedStr);
        }
    }

    // Log
    emit calculationDetail(LOG_EVENT_FINAL_RESULTS.arg(seatListStr, unresolvedListStr).arg(results.unfilledSeatCount()));
}

//Public:
/*!
 *  Returns the current election that the calculator is set to evaluate, or @c nullptr if none is set.
 *
 *  @sa Calculator(), and setElection().
 */
const Election* Calculator::election() const { return mElection; }

/*!
 *  Returns the current option set of the calculator.
 *
 *  @sa setOptions().
 */
Calculator::Options Calculator::options() const { return mOptions; }

/*!
 *  Sets the calculator to evaluate the Election @a election.
 *
 *  @sa election().
 */
void Calculator::setElection(const Election* election) { mElection = election; }

/*!
 *  Sets the calculator options set to @a options.
 *
 *  @sa options().
 */
void Calculator::setOptions(Options options) { mOptions = options; }

/*!
 *  Determines the outcome of the currently set election in accordance with the current options set
 *  and returns it as an ElectionResult.
 *
 *  If no election is set or the current one is invalid, a null ElectionResult is returned.
 *
 *  @sa isNull() and calculationDetail.
 */
ElectionResult Calculator::calculateResult()
{
    // Check for valid election
    if(!mElection || !mElection->isValid())
    {
        emit calculationDetail(LOG_EVENT_INVALID_ELECTION);
        return ElectionResult();
    }

    // Log start
    emit calculationDetail(LOG_EVENT_CALC_START.arg(mElection->name()) + '\n' + QString(120,'#'));

    // Note counts
    emit calculationDetail(LOG_EVENT_INPUT_COUNTS.arg(mElection->candidates().size())
                                                 .arg(mElection->ballots().size())
                                                 .arg(mElection->seatCount()));

    // Print out raw score rankings
    emit calculationDetail(LOG_EVENT_INITAL_RAW_RANKINGS + '\n' + createCandidateRankListString(mElection->scoreRankings()));

    // Pre-calculate head-to-heads
    emit calculationDetail(LOG_EVENT_CALC_HEAD_TO_HEAD);
    mHeadToHeadResults = std::make_unique<HeadToHeadResults>(mElection);

    // Results holder
    QList<Seat> processedSeats;

    // Active candidate rankings
    QList<Rank> candidateRankings = mElection->scoreRankings();

    for(int s = 0; s < mElection->seatCount(); s++)
    {
        emit calculationDetail(LOG_EVENT_FILLING_SEAT.arg(s));

        // Handle case of only one candidate remaining
        if(candidateRankings.size() == 1)
        {
            const QSet<QString>& frontCandidates = candidateRankings.at(0).candidates;
            if(frontCandidates.size() == 1)
            {
                emit calculationDetail(LOG_EVENT_DIRECT_SEAT_FILL);
                processedSeats.append(Seat(*frontCandidates.cbegin()));
                break;
            }
        }

        QString seatWinner;

        // Determine scoring round leaders based on raw score
        QualifierResult runoffQualifier = performRunoffQualifier(candidateRankings);

        // Check for an unresolved scoring round tie that prevented a runoff
        if(!runoffQualifier.isComplete())
        {
            emit calculationDetail(LOG_EVENT_NO_RUNOFF);

            // Check if runoff sim is possible
            if(mOptions.testFlag(Option::DefactoWinner) && runoffQualifier.hasFirstSeed())
            {
                if(checkForDefactoWinner(runoffQualifier.firstSeed(), runoffQualifier.overflow()))
                    seatWinner = runoffQualifier.firstSeed();

                emit calculationDetail(LOG_EVENT_DEFACTO_WINNER_SEAT_FILL.arg(seatWinner));
            }

            // Stop election
            processedSeats.append(Seat(seatWinner, runoffQualifier));
            break;
        }

        emit calculationDetail(LOG_EVENT_RUNOFF_CANDIDATES.arg(runoffQualifier.firstSeed(), runoffQualifier.secondSeed()));

        // Perform primary runoff
        emit calculationDetail(LOG_EVENT_PERFORM_PRIMARY_RUNOFF);
        seatWinner = performRunoff(runoffQualifier.seeds());

        // Check for unresolved runoff tie
        if(seatWinner.isNull())
        {
            processedSeats.append(runoffQualifier);
            break;
        }

        // Record seat winner
        processedSeats.append(Seat(seatWinner, runoffQualifier));

        /* Remove seat winner from remaining rankings
         *
         * It's known that the winner will always be in the first or second rank, but this
         * is done as a loop anyway for clarity and ease of rank erasure.
         */
        auto rItr = candidateRankings.begin();
        while(rItr != candidateRankings.end())
        {
            Rank& rank = *rItr;

            if(rank.candidates.contains(seatWinner))
            {
                if(rank.candidates.size() == 1)
                    candidateRankings.erase(rItr); // clazy:exclude=strict-iterators
                else
                    rank.candidates.remove(seatWinner);

                break;
            }

            rItr++;
        }
    }

    ElectionResult finalResults(mElection, processedSeats);

    // Note final results
    logElectionResults(finalResults);

    // Log finish
    emit calculationDetail(LOG_EVENT_CALC_FINISH + '\n' + QString(120,'-'));

    // Return final results
    return finalResults;
}

/*!
 *  @fn void Calculator::calculationDetail(const QString& detail)
 *
 *  This signal is emitted at various points during the execution of calculateResult().
 *
 *  It provides numerous details about the entire calculation process that are useful for logging and
 *  gaining insight into the STAR election procedure.
 *
 *  @sa calculateResult().
 */

}
