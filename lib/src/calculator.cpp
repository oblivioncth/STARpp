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
    mElection(election)
{}

Calculator::~Calculator() = default;

//-Instance Functions-------------------------------------------------------------------------------------------------
//Private:
QSet<QString> Calculator::determinePreliminaryLeaders()
{
    emit calculationDetail(LOG_EVENT_DETERMINE_PRELIMINARY_LEADERS);

    QSet<QString> leaders;

    // Handle tie & non-tie cases
    QSet<QString> nomineesInFirst = mElection->scoreRankings().front().nominees;
    if(nomineesInFirst.size() > 1) // First place tie
    {
        int firstPlaceScore = mElection->scoreRankings().front().value;

        if(nomineesInFirst.size() == 2) // Two-way
        {
            QString tieLogStr = LOG_EVENT_PRELIMINARY_FIRST_TIE_BENIGN.arg(firstPlaceScore) + '\n' + createNomineeGeneralSetString(nomineesInFirst);
            emit calculationDetail(tieLogStr);

            leaders = nomineesInFirst;
        }
        else // N-way
        {
            QString tieLogStr = LOG_EVENT_PRELIMINARY_FIRST_TIE.arg(nomineesInFirst.size()).arg(firstPlaceScore) + '\n' + createNomineeGeneralSetString(nomineesInFirst);
            emit calculationDetail(tieLogStr);

            // Tiebreak
            leaders = preliminaryCandidateTieReduction(nomineesInFirst, 2);
        }
    }
    else
    {
        // First place uncontested
        QString first = *nomineesInFirst.constBegin();
        emit calculationDetail(LOG_EVENT_PRELIMINARY_FIRST_NO_TIE.arg(first));
        leaders.insert(first);

        // Check second place
        QSet<QString> nomineesInSecond = mElection->scoreRankings().at(1).nominees;
        if(nomineesInSecond.size() > 1) // Second place tie
        {
            int secondPlaceScore = mElection->scoreRankings().at(1).value;

            QString tieLogStr = LOG_EVENT_PRELIMINARY_SECOND_TIE.arg(nomineesInSecond.size()).arg(secondPlaceScore) + '\n' + createNomineeGeneralSetString(nomineesInSecond);
            emit calculationDetail(tieLogStr);

            // Tiebreak
            QSet<QString> secondPlaceTiebreak = preliminaryCandidateTieReduction(nomineesInSecond, 1);

            // Insert all from the tiebreak, fully resolved or not
            leaders.unite(secondPlaceTiebreak);
        }
        else
        {
            // Second place uncontested
            QString second = *nomineesInSecond.constBegin();
            emit calculationDetail(LOG_EVENT_PRELIMINARY_SECOND_NO_TIE.arg(second));
            leaders.insert(second);
        }
    }

    emit calculationDetail(LOG_EVENT_PRELIMINARY_LEADERS + '\n' + createNomineeToalScoreSetString(leaders));
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
                // Randomly choose a winner
                emit calculationDetail(LOG_EVENT_PRIMARY_CHOOSING_RANDOM_WINNER);
                winner = breakTieRandom(cTied);
            }
        }
    }

    // Note results
    if(winner.isNull())
        ; // TODO: Add when supporting true ties
    else
        emit calculationDetail(LOG_EVENT_PRIMARY_WINNER.arg(winner));

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
                // Randomly choose a candidate to cut
                toBeCut = breakTieRandom(leastFiveStars);
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
    emit calculationDetail(LOG_EVENT_PPRELIMINARY_TIE_REDUCTION_RESULT + '\n' + createNomineeToalScoreSetString(candidates));
    return candidates;
}

QList<Rank> Calculator::rankByScore(const QSet<QString>& nominees, Rank::Order order) const
{
    /* Determine aggregate score of nominee list
     * Redoing this with the provided sub-list is more straight forward than trying to manipulate
     * the full score rankings that are part of the Election
     */
    emit calculationDetail(LOG_EVENT_RANK_BY_SCORE.arg(ENUM_NAME(order)));
    QMap<QString, int> totalScoreMap;

    for(const QString& nominee : nominees)
        totalScoreMap[nominee] = mElection->totalScore(nominee);

    // Create sorted rank list
    QList<Rank> scoreRanks = Rank::rankSort(totalScoreMap, order);

    emit calculationDetail(LOG_EVENT_RANKINGS_SCORE + '\n' + createNomineeRankListString(scoreRanks));
    return scoreRanks;
}

QList<Rank> Calculator::rankByVotesOfMaxScore(const QSet<QString>& nominees, Rank::Order order) const
{
    // Determine aggregate max votes of nominee list
    emit calculationDetail(LOG_EVENT_RANK_BY_VOTES_OF_MAX_SCORE.arg(ENUM_NAME(order)));
    QMap<QString, int> totalMaxVotesMap;

    for(const QString& nominee : nominees)
    {
        uint maxVoteCount = 0;

        for(const Star::Election::Ballot& ballot : mElection->ballots())
            if(ballot.score(nominee) == 5)
                maxVoteCount++;

        totalMaxVotesMap[nominee] = maxVoteCount;
    }

    // Create sorted rank list
    QList<Rank> maxVoteRanks = Rank::rankSort(totalMaxVotesMap, order);

    emit calculationDetail(LOG_EVENT_RANKINGS_VOTES_OF_MAX_SCORE + '\n' + createNomineeRankListString(maxVoteRanks));
    return maxVoteRanks;
}

QList<Rank> Calculator::rankByHeadToHeadLosses(const QSet<QString>& nominees, const HeadToHeadResults* hth, Rank::Order order) const
{
    // Create losses map
    emit calculationDetail(LOG_EVENT_RANK_BY_HEAD_TO_HEAD_LOSSES.arg(ENUM_NAME(order)));
    QMap<QString, int> losses;

    for(const QString& c : nominees)
        losses[c] = hth->losses(c);

    // Create sorted wins losses list
    QList<Rank> headToHeadLossesRanks = Rank::rankSort(losses, order);

    emit calculationDetail(LOG_EVENT_RANKINGS_HEAD_TO_HEAD_LOSSES + '\n' + createNomineeRankListString(headToHeadLossesRanks));
    return headToHeadLossesRanks;
}

QList<Rank> Calculator::rankByHeadToHeadPreferences(const QSet<QString>& nominees, const HeadToHeadResults* hth, Rank::Order order) const
{
    // Determine aggregate face-off wins of nominees list
    emit calculationDetail(LOG_EVENT_RANK_BY_HEAD_TO_HEAD_PREFERENCES.arg(ENUM_NAME(order)));

    // Create pref count map
    QMap<QString, int> preferences;
    for(const QString& c : nominees)
        preferences[c] = hth->preferences(c);

    // Create scoped & sorted wins list
    QList<Rank> headToHeadPrefCountRanks = Rank::rankSort(preferences, order);

    emit calculationDetail(LOG_EVENT_RANKINGS_HEAD_TO_HEAD_PREFERENCES + '\n' + createNomineeRankListString(headToHeadPrefCountRanks));
    return headToHeadPrefCountRanks;
}

QList<Rank> Calculator::rankByHeadToHeadMargin(const QSet<QString>& nominees, const HeadToHeadResults* hth, Rank::Order order) const
{
    // Determine aggregate face-off wins of nominees list
    emit calculationDetail(LOG_EVENT_RANK_BY_HEAD_TO_HEAD_MARGIN.arg(ENUM_NAME(order)));

    // Create pref count map
    QMap<QString, int> margins;
    for(const QString& c : nominees)
        margins[c] = hth->margin(c);

    // Create scoped & sorted wins list
    QList<Rank> headToHeadMarginRanks = Rank::rankSort(margins, order);

    emit calculationDetail(LOG_EVENT_RANKINGS_HEAD_TO_HEAD_MARGIN + '\n' + createNomineeRankListString(headToHeadMarginRanks));
    return headToHeadMarginRanks;
}

QSet<QString> Calculator::rankBasedTiebreak(const QList<Rank>& rankings, const QString& note) const
{
    // Break a tie by using the provided rankings
    emit calculationDetail(note);
    QSet<QString> tieBreak = rankings.front().nominees;

    emit calculationDetail(LOG_EVENT_BREAK_RESULT.arg(Qx::String::join(tieBreak, ", ")));
    return tieBreak;
}

QSet<QString> Calculator::breakTieMostFiveStar(const QSet<QString>& nominees) const
{
    return rankBasedTiebreak(rankByVotesOfMaxScore(nominees, Rank::Descending), LOG_EVENT_BREAK_TIE_MOST_FIVE_STAR.arg(nominees.size()));
}

QSet<QString> Calculator::breakTieLeastFiveStar(const QSet<QString>& nominees) const
{
    return rankBasedTiebreak(rankByVotesOfMaxScore(nominees, Rank::Ascending), LOG_EVENT_BREAK_TIE_LEAST_FIVE_STAR.arg(nominees.size()));
}

QSet<QString> Calculator::breakTieMostHeadToHeadLosses(const QSet<QString>& nominees, const HeadToHeadResults* hth) const
{
    return rankBasedTiebreak(rankByHeadToHeadLosses(nominees, hth, Rank::Descending), LOG_EVENT_BREAK_TIE_MOST_HEAD_TO_HEAD_LOSSES.arg(nominees.size()));
}

QSet<QString> Calculator::breakTieLeastHeadToHeadPreferences(const QSet<QString>& nominees, const HeadToHeadResults* hth) const
{
    return rankBasedTiebreak(rankByHeadToHeadPreferences(nominees, hth, Rank::Ascending), LOG_EVENT_BREAK_TIE_LEAST_HEAD_TO_HEAD_PREFERENCES.arg(nominees.size()));
}

QSet<QString> Calculator::breakTieSmallestHeadToHeadMargin(const QSet<QString>& nominees, const HeadToHeadResults* hth) const
{
    return rankBasedTiebreak(rankByHeadToHeadMargin(nominees, hth, Rank::Ascending), LOG_EVENT_BREAK_TIE_SMALLEST_HEAD_TO_HEAD_MARGIN.arg(nominees.size()));
}

QSet<QString> Calculator::breakTieHighestScore(const QSet<QString>& nominees) const
{
    return rankBasedTiebreak(rankByScore(nominees, Rank::Descending), LOG_EVENT_BREAK_TIE_HIGHEST_SCORE.arg(nominees.size()));
}

// TODO: Remove
//QSet<QString> Calculator::breakTieCondorcetProtocol(const QSet<QString>& nominees, const HeadToHeadResults* hth)
//{
//    emit calculationDetail(LOG_EVENT_BREAK_EXTENDED_TIE.arg(nominees.size()).arg(ENUM_NAME(Condorcet)));
//    emit calculationDetail(LOG_EVENT_CONDORCET_START_STAGES);

//    // Break tie by least head-to-head preferences
//    QSet<QString> leastPreferences = breakTieLeastHeadToHeadPreferences(nominees, hth);
//    if(leastPreferences.size() == 1)
//        return leastPreferences;
//    else
//    {
//        // Remove any candidates handled by the previous break if necessary
//        std::unique_ptr<HeadToHeadResults> reducedHth;
//        if(leastPreferences.size() < nominees.size())
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

QString Calculator::breakTieRandom(const QSet<QString>& nominees) const
{
    emit calculationDetail(LOG_EVENT_BREAK_TIE_RANDOM.arg(nominees.size()));

    /* Randomly select a winner/loser of the tiebreak
     *
     * There is already some level of randomness since the iteration order of a set is undefined, but this is not enough on its own.
     */
    quint32 selection = QRandomGenerator::global()->bounded(nominees.size());

    auto itr = nominees.constBegin();
    for(qsizetype i = 0; i < selection; i++)
        itr ++;

    return *itr;
}

QString Calculator::createNomineeGeneralSetString(const QSet<QString>& nominees) const
{
    if(nominees.isEmpty())
        return QString();

    return Qx::String::join(nominees, "\n", "\t- ");
}

QString Calculator::createNomineeToalScoreSetString(const QSet<QString>& nominees) const
{
    if(nominees.isEmpty())
        return QString();

    QString listStr;

    auto itr = nominees.constBegin();
    while(itr != nominees.constEnd())
    {
        const QString& nominee = *itr;
        QString nomineeStr = LIST_ITEM_NOMINEE_TOTAL_SCORE.arg(nominee).arg(mElection->totalScore(nominee));

        if(++itr != nominees.constEnd())
            nomineeStr += '\n';
        listStr.append(nomineeStr);
    }

    return listStr;
}

QString Calculator::createNomineeRankListString(const QList<Rank>& ranks) const
{
    if(ranks.isEmpty())
        return QString();

    QString listStr;
    qsizetype lastIdx = ranks.size() - 1;
    for(qsizetype i = 0; i < ranks.size(); i++)
    {
        const Rank& rank = ranks.at(i);
        QString nomineeStr = LIST_ITEM_RANK.arg(i).arg(Qx::String::join(rank.nominees, R"(", ")")).arg(rank.value);
        if(i != lastIdx)
            nomineeStr += '\n';
        listStr.append(nomineeStr);
    }

    return listStr;
}

//Public:
const Election* Calculator::election() const { return mElection; }
void Calculator::setElection(const Election* election) { mElection = election; }

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
    emit calculationDetail(LOG_EVENT_INPUT_COUNTS.arg(mElection->nominees().size()).arg(mElection->ballots().size()));

    // Print out raw score rankings
    emit calculationDetail(LOG_EVENT_INITAL_RAW_RANKINGS + '\n' + createNomineeRankListString(mElection->scoreRankings()));

    // Pre-calculate head-to-heads
    emit calculationDetail(LOG_EVENT_CALC_HEAD_TO_HEAD);
    mHeadToHeadResults = std::make_unique<HeadToHeadResults>(mElection);

    // Determine preliminary leaders based on raw score
    QSet<QString> preliminaryLeaders = determinePreliminaryLeaders();

    // Check for an unresolved preliminary tie that prevented a runoff
    if(preliminaryLeaders.size() != 2)
    {
        // TODO: Fill out when disabling random tiebreaker is added
    }

    // Segment out runoff candidates
    QPair<QString, QString> runoffCandidates;
    auto pItr = preliminaryLeaders.cbegin();
    runoffCandidates.first = *(pItr++);
    runoffCandidates.second = *pItr;
    emit calculationDetail(LOG_EVENT_RUNOFF_CANDIDATES.arg(runoffCandidates.first, runoffCandidates.second));

    // Perform primary runoff
    QString winner = performPrimaryRunoff(runoffCandidates);

    // Check for unresolved runoff tie
    if(winner.isNull())
    {
        // TODO: Fill out when disabling random tiebreaker is added
    }

    // Log finish
    emit calculationDetail(LOG_EVENT_CALC_FINISH + '\n' + QString(120,'-'));

    // Return final results
    return ElectionResult(mElection, winner);
}


}
