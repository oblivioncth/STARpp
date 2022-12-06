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

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
Calculator::Calculator(const Election* election) :
    mElection(election),
    mOptions(Option::NoOptions)
{}

Calculator::~Calculator() = default;

//-Instance Functions-------------------------------------------------------------------------------------------------
//Private:
QSet<QString> Calculator::determinePreliminaryLeaders(const QList<Rank>& scoreRankings)
{
    emit calculationDetail(LOG_EVENT_DETERMINE_PRELIMINARY_LEADERS);

    QSet<QString> leaders;

    // Handle tie & non-tie cases
    QSet<QString> candidatesInFirst = scoreRankings.front().candidates;
    if(candidatesInFirst.size() > 1) // First place tie
    {
        int firstPlaceScore = scoreRankings.front().value;

        if(candidatesInFirst.size() == 2) // Two-way
        {
            QString tieLogStr = LOG_EVENT_PRELIMINARY_FIRST_TIE_BENIGN.arg(firstPlaceScore) + '\n' + createCandidateGeneralSetString(candidatesInFirst);
            emit calculationDetail(tieLogStr);

            leaders = candidatesInFirst;
        }
        else // N-way
        {
            QString tieLogStr = LOG_EVENT_PRELIMINARY_FIRST_TIE.arg(candidatesInFirst.size()).arg(firstPlaceScore) + '\n' + createCandidateGeneralSetString(candidatesInFirst);
            emit calculationDetail(tieLogStr);

            // Tiebreak
            leaders = preliminaryCandidateTieReduction(candidatesInFirst, 2);
        }
    }
    else
    {
        // First place uncontested
        QString first = *candidatesInFirst.constBegin();
        emit calculationDetail(LOG_EVENT_PRELIMINARY_FIRST_NO_TIE.arg(first));
        leaders.insert(first);

        // Check second place
        QSet<QString> candidatesInSecond = scoreRankings.at(1).candidates;
        if(candidatesInSecond.size() > 1) // Second place tie
        {
            int secondPlaceScore = scoreRankings.at(1).value;

            QString tieLogStr = LOG_EVENT_PRELIMINARY_SECOND_TIE.arg(candidatesInSecond.size()).arg(secondPlaceScore) + '\n' + createCandidateGeneralSetString(candidatesInSecond);
            emit calculationDetail(tieLogStr);

            // Tiebreak
            QSet<QString> secondPlaceTiebreak = preliminaryCandidateTieReduction(candidatesInSecond, 1);

            // Insert all from the tiebreak, fully resolved or not
            leaders.unite(secondPlaceTiebreak);
        }
        else
        {
            // Second place uncontested
            QString second = *candidatesInSecond.constBegin();
            emit calculationDetail(LOG_EVENT_PRELIMINARY_SECOND_NO_TIE.arg(second));
            leaders.insert(second);
        }
    }

    emit calculationDetail(LOG_EVENT_PRELIMINARY_LEADERS + '\n' + createCandidateToalScoreSetString(leaders));
    return leaders;
}

QString Calculator::performPrimaryRunoff(QPair<QString, QString> candidates) const
{
    emit calculationDetail(LOG_EVENT_PERFORM_PRIMARY_RUNOFF);

    // Check for clear winner
    emit calculationDetail(LOG_EVENT_PRIMARY_HEAD_TO_HEAD_WINNER_CHECK);
    QString winner = mHeadToHeadResults->winner(candidates.first, candidates.second);
    if(winner.isNull())
    {
        emit calculationDetail(LOG_EVENT_PRIMARY_TIE);
        QSet<QString> cTied = {candidates.first, candidates.second};

        // Try to break tie by original score
        emit calculationDetail(LOG_EVENT_PRIMARY_HIGHER_SCORE_CHECK);
        QSet<QString> highestScore = breakTieHighestScore(cTied);
        if(highestScore.size() == 1)
            winner = *highestScore.cbegin();
        else
        {
            // Try to break tie by five star votes
            emit calculationDetail(LOG_EVENT_PRIMARY_MORE_FIVE_STAR_CHECK);
            QSet<QString> mostFiveStar = breakTieMostFiveStar(cTied);
            if(mostFiveStar.size() == 1)
                winner = *mostFiveStar.cbegin();
            else
            {
                // Randomly choose a winner if allowed
                if(!mOptions.testFlag(Option::AllowTrueTies))
                {
                    emit calculationDetail(LOG_EVENT_PRIMARY_CHOOSING_RANDOM_WINNER);
                    winner = breakTieRandom(cTied);
                }
                else
                    emit calculationDetail(LOG_EVENT_PRIMARY_NO_RANDOM);
            }
        }
    }

    // Note results
    emit calculationDetail(winner.isNull() ? LOG_EVENT_PRIMARY_UNRESOLVED : LOG_EVENT_PRIMARY_WINNER.arg(winner));

    // Return result
    return winner;
}

QSet<QString> Calculator::preliminaryCandidateTieReduction(QSet<QString> candidates, qsizetype desiredCount) const
{
    // This function should never be called in this situation, but account for it anyway
    if(candidates.size() <= desiredCount)
    {
        qWarning("called with a set of candidates that is already at or less than the desired count.");
        return candidates;
    }

    // Get copy of head-to-head results and reduce it to only include tied candidates
    HeadToHeadResults relevantHthResults = *mHeadToHeadResults;
    relevantHthResults.narrow(candidates, HeadToHeadResults::Inclusive);

    // Eliminate the weakest candidates in turn until at the desired amount
    emit calculationDetail(LOG_EVENT_PRELIMINARY_TIE_REDUCTION.arg(candidates.size()).arg(desiredCount));
    while(candidates.size() != desiredCount)
    {
        QString toBeCut;

        // Check for clear head-to-head loser
        QSet<QString> mostLosses = breakTieMostHeadToHeadLosses(candidates, &relevantHthResults);
        if(mostLosses.size() == 1)
            toBeCut = *mostLosses.begin();
        else
        {
            // Check for clear Five Star loser
            QSet<QString> leastFiveStars = breakTieLeastFiveStar(mostLosses);
            if(leastFiveStars.size() == 1)
                toBeCut = *leastFiveStars.begin();
            else
            {
                QSet<QString> remainingPool = leastFiveStars;

                // Follow Condorcet protocol if enabled
                if(mOptions.testFlag(Option::CondorcetProtocol))
                {
                    // Check for clear preference loser
                    QSet<QString> leastPreferences = breakTieLeastHeadToHeadPreferences(leastFiveStars, &relevantHthResults);
                    if(leastPreferences.size() == 1)
                        toBeCut = *leastPreferences.begin();
                    else
                    {
                        // Check for clear head-to-head win margin loser
                        QSet<QString> smallestMargin = breakTieSmallestHeadToHeadMargin(leastPreferences, &relevantHthResults);
                        if(smallestMargin.size() == 1)
                            toBeCut = *smallestMargin.begin();
                        else
                            remainingPool = smallestMargin;
                    }

                }

                // Randomly choose a candidate to cut if required and allowed
                if(toBeCut.isNull() && !mOptions.testFlag(Option::AllowTrueTies))
                    toBeCut = breakTieRandom(remainingPool);
                else
                    emit calculationDetail(LOG_EVENT_PRELIMINARY_NO_RANDOM);
            }
        }

        // Cut candidate if the tie was resolvable
        if(!toBeCut.isNull())
        {
            emit calculationDetail(LOG_EVENT_PPRELIMINARY_TIE_CUT_CANDIDATE.arg(toBeCut));
            candidates.remove(toBeCut);
            relevantHthResults.narrow({toBeCut}, HeadToHeadResults::Exclusive);
        }
        else
        {
            emit calculationDetail(LOG_EVENT_PPRELIMINARY_TIE_REDUCTION_UNSUCCESSFUL);
            break;
        }
    }

    // Return the reduced candidate set, ideally at target size
    emit calculationDetail(LOG_EVENT_PPRELIMINARY_TIE_REDUCTION_RESULT + '\n' + createCandidateToalScoreSetString(candidates));
    return candidates;
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

QSet<QString> Calculator::breakTieLeastFiveStar(const QSet<QString>& candidates) const
{
    return rankBasedTiebreak(rankByVotesOfMaxScore(candidates, Rank::Ascending), LOG_EVENT_BREAK_TIE_LEAST_FIVE_STAR.arg(candidates.size()));
}

QSet<QString> Calculator::breakTieMostHeadToHeadLosses(const QSet<QString>& candidates, const HeadToHeadResults* hth) const
{
    return rankBasedTiebreak(rankByHeadToHeadLosses(candidates, hth, Rank::Descending), LOG_EVENT_BREAK_TIE_MOST_HEAD_TO_HEAD_LOSSES.arg(candidates.size()));
}

QSet<QString> Calculator::breakTieLeastHeadToHeadPreferences(const QSet<QString>& candidates, const HeadToHeadResults* hth) const
{
    return rankBasedTiebreak(rankByHeadToHeadPreferences(candidates, hth, Rank::Ascending), LOG_EVENT_BREAK_TIE_LEAST_HEAD_TO_HEAD_PREFERENCES.arg(candidates.size()));
}

QSet<QString> Calculator::breakTieSmallestHeadToHeadMargin(const QSet<QString>& candidates, const HeadToHeadResults* hth) const
{
    return rankBasedTiebreak(rankByHeadToHeadMargin(candidates, hth, Rank::Ascending), LOG_EVENT_BREAK_TIE_SMALLEST_HEAD_TO_HEAD_MARGIN.arg(candidates.size()));
}

QSet<QString> Calculator::breakTieHighestScore(const QSet<QString>& candidates) const
{
    return rankBasedTiebreak(rankByScore(candidates, Rank::Descending), LOG_EVENT_BREAK_TIE_HIGHEST_SCORE.arg(candidates.size()));
}

// TODO: Remove
//QSet<QString> Calculator::breakTieCondorcetProtocol(const QSet<QString>& candidates, const HeadToHeadResults* hth)
//{
//    emit calculationDetail(LOG_EVENT_BREAK_EXTENDED_TIE.arg(candidates.size()).arg(ENUM_NAME(Condorcet)));
//    emit calculationDetail(LOG_EVENT_CONDORCET_START_STAGES);

//    // Break tie by least head-to-head preferences
//    QSet<QString> leastPreferences = breakTieLeastHeadToHeadPreferences(candidates, hth);
//    if(leastPreferences.size() == 1)
//        return leastPreferences;
//    else
//    {
//        // Remove any candidates handled by the previous break if necessary
//        std::unique_ptr<HeadToHeadResults> reducedHth;
//        if(leastPreferences.size() < candidates.size())
//        {
//            reducedHth = std::make_unique<HeadToHeadResults>(*hth);
//            reducedHth->narrow()
//        }


//        // Check for clear Five Star loser
//        QSet<QString> leastFiveStars = breakTieLeastFiveStar(mostLosses);
//        if(leastFiveStars.size() == 1)
//            toBeCut = *leastFiveStars.begin();
//        else
//        {
//            // Randomly choose a candidate to cut
//            toBeCut = breakTieRandom(leastFiveStars);
//        }
//    }
//}

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

void Calculator::logElectionResults(const ElectionResult& results) const
{
    // Create filled seats string
    QString seatListStr;
    for(qsizetype i = 0; i < results.winners().size(); i++)
    {
        QString seatWinner = results.winners().at(i);
        QString seatStr = LIST_ITEM_SEAT.arg(i).arg(seatWinner);
        seatStr += '\n';
        seatListStr.append(seatStr);
    }

    // Create unresolved candidates string
    QString unresolvedListStr;
    const QSet<QString>& cUnresolved = results.unresolvedCandidates();
    for(auto itr = cUnresolved.cbegin(); itr != cUnresolved.cend(); itr++)
    {
        QString candidate = *itr;
        QString unresolvedStr = LIST_ITEM_UNRESOLVED.arg(candidate);
        unresolvedStr += '\n';
        unresolvedListStr.append(unresolvedListStr);
    }

    // Log
    emit calculationDetail(LOG_EVENT_FINAL_RESULTS.arg(seatListStr, unresolvedListStr).arg(results.unfilledSeatCount()));
}

//Public:
const Election* Calculator::election() const { return mElection; }
Calculator::Options Calculator::options() const { return mOptions; }
void Calculator::setElection(const Election* election) { mElection = election; }
void Calculator::setOptions(Options options) { mOptions = options; }

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

    // Results holders
    QStringList winners;
    QSet<QString> unresolvedCandidates;

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
                winners.append(*frontCandidates.cbegin());
                break;
            }
        }

        QString seatWinner;

        // Determine preliminary leaders based on raw score
        QSet<QString> preliminaryLeaders = determinePreliminaryLeaders(candidateRankings);

        // Check for an unresolved preliminary tie that prevented a runoff
        if(preliminaryLeaders.size() != 2)
        {
            emit calculationDetail(LOG_EVENT_NO_RUNOFF);
            unresolvedCandidates = preliminaryLeaders;
            break;
        }

        // Segment out runoff candidates
        QPair<QString, QString> runoffCandidates;
        auto pItr = preliminaryLeaders.cbegin();
        runoffCandidates.first = *(pItr++);
        runoffCandidates.second = *pItr;
        emit calculationDetail(LOG_EVENT_RUNOFF_CANDIDATES.arg(runoffCandidates.first, runoffCandidates.second));

        // Perform primary runoff
        seatWinner = performPrimaryRunoff(runoffCandidates);

        // Check for unresolved runoff tie
        if(seatWinner.isNull())
            unresolvedCandidates = preliminaryLeaders;

        // Record seat winner
        winners.append(seatWinner);

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
                    candidateRankings.erase(rItr);
                else
                    rank.candidates.remove(seatWinner);

                break;
            }

            rItr++;
        }
    }

    ElectionResult finalResults(mElection, winners, unresolvedCandidates);

    // Note final results
    logElectionResults(finalResults);

    // Log finish
    emit calculationDetail(LOG_EVENT_CALC_FINISH + '\n' + QString(120,'-'));

    // Return final results
    return finalResults;
}


}
