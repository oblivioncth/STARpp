// Unit Include
#include "star/calculator.h"

// Qx Includes
#include <qx/core/qx-string.h>
#include <qx/core/qx-algorithm.h>

// magic_enum Includes
#include <magic_enum.hpp>

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
    mExtraTiebreakMethod(std::nullopt)
{}

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
            QPair<QSet<QString>, QSet<QString>> firstPlaceTiebreak = breakScoreTie(nomineesInFirst);

            // Need at least 2 nominees
            leaders.unite(firstPlaceTiebreak.first);
            if(leaders.size() < 2)
                leaders.unite(firstPlaceTiebreak.second); // If there are ties for second pref rank, they all just go on
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
            QPair<QSet<QString>, QSet<QString>> secondPlaceTiebreak = breakScoreTie(nomineesInSecond);

            // Insert all in first place of preference rankings, ties or not
            leaders.unite(secondPlaceTiebreak.first);
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

QPair<QSet<QString>, QSet<QString>> Calculator::performPrimaryRunoff(const QSet<QString>& preliminaryLeaders)
{
    emit calculationDetail(LOG_EVENT_PERFORM_PRIMARY_RUNOFF);

    // Buffers
    QSet<QString> winners;
    QSet<QString> runnerUps;

    // Determine preference ranking
    emit calculationDetail(LOG_EVENT_PRIMARY_PREF_RANK_SORT);
    QList<Rank> prefRanks = rankByPreference(preliminaryLeaders);

    // Handle tie & non-tie cases
    QSet<QString> nomineesInFirst = prefRanks.front().nominees;
    if(nomineesInFirst.size() > 1) // First place tie
    {
        int firstPlacePrefCount = prefRanks.front().value;
        QString tieLogStr = LOG_EVENT_PRIMARY_FIRST_TIE.arg(nomineesInFirst.size()).arg(firstPlacePrefCount) + '\n' + createNomineeGeneralSetString(nomineesInFirst);
        emit calculationDetail(tieLogStr);

        // Tiebreak
        QPair<QSet<QString>, QSet<QString>> firstPlaceTiebreak = breakPreferenceTie(nomineesInFirst);

        // Determine winners/runner-ups
        winners = firstPlaceTiebreak.first;
        if(!firstPlaceTiebreak.second.isEmpty())
            runnerUps = firstPlaceTiebreak.second;
        else
        {
            emit calculationDetail(LOG_EVENT_PRIMARY_TIEBREAK_FAIL);

            // Get second place from preference rankings, if available
            if(prefRanks.size() > 1)
            {
                QSet<QString> nomineesInSecond = prefRanks.at(1).nominees;
                runnerUps = nomineesInSecond.size() > 1 ? breakPreferenceTie(nomineesInSecond).first : nomineesInSecond;
            }
            else
                emit calculationDetail(LOG_EVENT_PRIMARY_TIEBREAK_FAIL_NO_FALLBACK);
        }
    }
    else
    {
        // First place uncontested
        QString first = *nomineesInFirst.constBegin();
        emit calculationDetail(LOG_EVENT_PRIMARY_FIRST_NO_TIE.arg(first));
        winners = {first};

        // Check second place
        QSet<QString> nomineesInSecond = prefRanks.at(1).nominees;
        if(nomineesInSecond.size() > 1) // Second place tie
        {
            int secondPlacePrefCount = prefRanks.at(1).value;
            QString tieLogStr = LOG_EVENT_PRIMARY_SECOND_TIE.arg(nomineesInSecond.size()).arg(secondPlacePrefCount) + '\n' + createNomineeGeneralSetString(nomineesInSecond);
            emit calculationDetail(tieLogStr);

            // Tiebreak
            QPair<QSet<QString>, QSet<QString>> secondPlaceTiebreak = breakPreferenceTie(nomineesInSecond);
            runnerUps = secondPlaceTiebreak.first;
        }
        else
        {
            // Second place uncontested
            QString second = *nomineesInSecond.constBegin();
            emit calculationDetail(LOG_EVENT_PRIMARY_SECOND_NO_TIE.arg(second));
            runnerUps = {second};
        }
    }

    // Note initial results
    emit calculationDetail(LOG_EVENT_INITIAL_RESULT_WINNERS.arg(Qx::String::join(winners, ", ")));
    emit calculationDetail(LOG_EVENT_INITIAL_RESULT_RUNNERUPS.arg(Qx::String::join(runnerUps, ", ")));

    // Return result
    return {winners, runnerUps};
}

QPair<QSet<QString>, QSet<QString>> Calculator::performExtendedTiebreak(QSet<QString> winners, QSet<QString> runnerUps, ExtendedTiebreakMethod method)
{
    Q_ASSERT(winners.size() > 1 || runnerUps.size() > 1);

    // Get head-to-head results of remaining candidates
    mHeadToHeadMaps = createHeadToHeadMaps(winners + runnerUps);

    // Determine method function
    std::function<QPair<QSet<QString>, QSet<QString>>(QSet<QString>)> methodFn;
    switch(method)
    {
        case FiveStar:
            methodFn = [this](QSet<QString> nominees){ return breakExtendedTieFiveStar(nominees); };
            break;
        case HTHWins:
            methodFn = [this](QSet<QString> nominees){ return breakExtendedTieHeadToHeadWins(nominees); };
            break;

        default:
            qFatal("Unhandled extended tiebreak method");
    }

    // If there is still a tie, check effect of extended tiebreak
    if(winners.size() > 1)
    {
        emit calculationDetail(LOG_EVENT_INITIAL_RESULT_WINNER_TIE);

        // Tiebreak
        QPair<QSet<QString>, QSet<QString>> firstPlaceTiebreak = methodFn(winners);

        // Determine winners/runner-ups
        winners = firstPlaceTiebreak.first;
        if(!firstPlaceTiebreak.second.isEmpty())
            runnerUps = firstPlaceTiebreak.second;
        else
        {
            emit calculationDetail(LOG_EVENT_EXTENDED_TIEBREAK_FAIL);

            // Get second place from original second place
            runnerUps = runnerUps.size() > 1 ? methodFn(runnerUps).first : runnerUps;
        }

    }
    else if(runnerUps.size() > 1)
    {
        emit calculationDetail(LOG_EVENT_INITIAL_RESULT_RUNNERUP_TIE);

        // Tiebreak
        QPair<QSet<QString>, QSet<QString>> secondPlaceTiebreak = methodFn(runnerUps);
        runnerUps = secondPlaceTiebreak.first;
    }

    // Note results
    emit calculationDetail(LOG_EVENT_EXTENDED_TIEBREAK_WINNERS.arg(Qx::String::join(winners, ", ")));
    emit calculationDetail(LOG_EVENT_EXTENDED_TIEBREAK_RUNNERUPS.arg(Qx::String::join(runnerUps, ", ")));

    // Return the result
    return {winners, runnerUps};
}

Calculator::HeadToHeadMaps Calculator::createHeadToHeadMaps(const QSet<QString>& nominees)
{
    // Determine aggregate face-off wins of nominees list
    emit calculationDetail(LOG_EVENT_CREATE_HEAD_TO_HEAD_MAPS);
    HeadToHeadMaps maps;

    // Add all nominees with an initial values of zero, since some might not be updated due to ties
    for(const QString& nominee : nominees)
    {
        maps.wins[nominee] = 0;
        maps.prefCounts[nominee] = 0;
        maps.margins[nominee] = 0;
    }

    // Create a full nominees list with the nominees under consideration at the top
    QStringList otherNominees = mElection->nominees();
    for(const QString n : nominees)
        otherNominees.removeAll(n);
    QStringList faceOffQueue = QList<QString>(nominees.cbegin(), nominees.cend()) + otherNominees;

    // Perform face-offs (restricted to only those involving nominees under consideration)
    QString test = *(faceOffQueue.constEnd() - 1);
    for(auto itrA = faceOffQueue.constBegin(); itrA != faceOffQueue.constEnd() - otherNominees.size(); itrA++)
    {
        QString opponentA = *itrA;

        for(auto itrB = itrA + 1; itrB != faceOffQueue.constEnd(); itrB++)
        {
            QString opponentB = *itrB;

            emit calculationDetail(LOG_EVENT_CREATE_HEAD_TO_HEAD_PREF.arg(opponentA, opponentB));
            QList<Rank> prefRanks = rankByPreference({opponentA, opponentB});

            if(prefRanks.first().nominees.size() != 1)
            {
                int prefCount = prefRanks.first().value;
                emit calculationDetail(LOG_EVENT_LOG_EVENT_CREATE_HEAD_TO_HEAD_PREF_TIE.arg(prefCount));

                maps.prefCounts[opponentA] += prefCount;
                if(nominees.contains(opponentB)) // May not be under consideration
                    maps.prefCounts[opponentB] += prefCount;
            }
            else
            {
                QString winner = *prefRanks.first().nominees.constBegin();
                int winnerCount = prefRanks.first().value;
                QString loser = *prefRanks.last().nominees.constBegin();
                int loserCount = prefRanks.last().value;
                int margin = Qx::distance(winnerCount, loserCount);

                emit calculationDetail(LOG_EVENT_LOG_EVENT_CREATE_HEAD_TO_HEAD_PREF_WIN.arg(winner).arg(winnerCount).arg(loser).arg(loserCount));

                if(nominees.contains(winner))
                {
                    maps.wins[winner]++;
                    maps.prefCounts[winner] += winnerCount;
                    maps.margins[winner] += margin;
                }

                if(nominees.contains(loser))
                {
                    maps.prefCounts[loser] += loserCount;
                    maps.margins[loser] -= margin;
                }
            }
        }
    }

    return maps;
}

QList<Rank> Calculator::rankByPreference(const QSet<QString>& nominees)
{
    // Determine aggregate preference of nominee list
    emit calculationDetail(LOG_EVENT_RANK_BY_PREF);
    QMap<QString, int> totalPreferenceMap;

    /* Add all nominees with an initial preference count of 0, since some might not be preferred even
     * once, and therefore be missed by the next loop
     */
    for(const QString& nominee : nominees)
        totalPreferenceMap[nominee] = 0;

    for(const Star::Election::Ballot& ballot : mElection->ballots())
    {
        QString voterName = ballot.voter().anonymousName;
        QString pref = ballot.preference(nominees);
        if(!pref.isNull())
        {
            int newTotal = ++totalPreferenceMap[pref];
            emit calculationDetail(LOG_EVENT_RANK_BY_PREF_HAS_PREF.arg(voterName, pref).arg(newTotal));
        }
        else
            emit calculationDetail(LOG_EVENT_RANK_BY_PREF_NO_PREF.arg(voterName));
    }

    // Create sorted rank list
    QList<Rank> prefRanks =  Rank::rankSort(totalPreferenceMap);

    emit calculationDetail(LOG_EVENT_RANKINGS_PREF + '\n' + createNomineeRankListString(prefRanks));
    return prefRanks;
}

QList<Rank> Calculator::rankByScore(const QSet<QString>& nominees)
{
    /* Determine aggregate score of nominee list
     * Redoing this with the provided sub-list is more straight forward than trying to manipulate
     * the full score rankings that are part of the Election
     */
    emit calculationDetail(LOG_EVENT_RANK_BY_SCORE);
    QMap<QString, int> totalScoreMap;

    for(const QString& nominee : nominees)
        totalScoreMap[nominee] = mElection->totalScore(nominee);

    // Create sorted rank list
    QList<Rank> scoreRanks = Rank::rankSort(totalScoreMap);

    emit calculationDetail(LOG_EVENT_RANKINGS_SCORE + '\n' + createNomineeRankListString(scoreRanks));
    return scoreRanks;
}

QList<Rank> Calculator::rankByVotesOfMaxScore(const QSet<QString>& nominees)
{
    // Determine aggregate max votes of nominee list
    emit calculationDetail(LOG_EVENT_RANK_BY_VOTES_OF_MAX_SCORE);
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
    QList<Rank> maxVoteRanks = Rank::rankSort(totalMaxVotesMap);

    emit calculationDetail(LOG_EVENT_RANKINGS_VOTES_OF_MAX_SCORE + '\n' + createNomineeRankListString(maxVoteRanks));
    return maxVoteRanks;
}
QList<Rank> Calculator::rankByHeadToHeadWins(const QSet<QString>& nominees)
{
    // Determine aggregate face-off wins of nominees list
    emit calculationDetail(LOG_EVENT_RANK_BY_HEAD_TO_HEAD_WINS);

    // Copy wins map, remove all but the specified nominees
    QMap<QString, int> wins = mHeadToHeadMaps.wins;
    wins.removeIf([&nominees](QMap<QString, int>::iterator itr){
        return !nominees.contains(itr.key());
    });

    // Create scoped & sorted wins list
    QList<Rank> headToHeadWinsRanks = Rank::rankSort(wins);

    emit calculationDetail(LOG_EVENT_RANKINGS_HEAD_TO_HEAD_WINS + '\n' + createNomineeRankListString(headToHeadWinsRanks));
    return headToHeadWinsRanks;
}

QPair<QSet<QString>, QSet<QString>> Calculator::rankBasedTiebreak(const QList<Rank>& rankings, const QString& note)
{
    // Check number of times a nominee is preferred to break tie
    emit calculationDetail(note);
    QPair<QSet<QString>, QSet<QString>> tieBreak(rankings.front().nominees, rankings.size() > 1 ? rankings.at(1).nominees : QSet<QString>());

    emit calculationDetail(LOG_EVENT_BREAK_RESULT.arg(Qx::String::join(tieBreak.first, ", "), Qx::String::join(tieBreak.second, ", ")));
    return tieBreak;
}

QPair<QSet<QString>, QSet<QString>> Calculator::breakScoreTie(const QSet<QString>& nominees)
{
    // Check number of times a nominee is preferred to break tie
    return rankBasedTiebreak(rankByPreference(nominees), LOG_EVENT_BREAK_SCORE_TIE.arg(nominees.size()));
}

QPair<QSet<QString>, QSet<QString>> Calculator::breakPreferenceTie(const QSet<QString>& nominees)
{
    // Check score to break tie
    return rankBasedTiebreak(rankByScore(nominees), LOG_EVENT_BREAK_PREF_TIE.arg(nominees.size()));
}

QPair<QSet<QString>, QSet<QString>> Calculator::breakExtendedTieFiveStar(const QSet<QString>& nominees)
{
    // Check number of times a nominee was given the maximum score possible to break tie
    return rankBasedTiebreak(rankByVotesOfMaxScore(nominees), LOG_EVENT_BREAK_EXTENDED_TIE.arg(nominees.size()).arg(ENUM_NAME(FiveStar)));
}

QPair<QSet<QString>, QSet<QString>> Calculator::breakExtendedTieHeadToHeadWins(const QSet<QString>& nominees)
{
    // Perform a face-off of each nominee and see which one has the most head-to-head wins
    return rankBasedTiebreak(rankByHeadToHeadWins(nominees), LOG_EVENT_BREAK_EXTENDED_TIE.arg(nominees.size()).arg(ENUM_NAME(HTHWins)));
}

QString Calculator::createNomineeGeneralSetString(const QSet<QString>& nominees)
{
    if(nominees.isEmpty())
        return QString();

    return Qx::String::join(nominees, "\n", "\t- ");
}

QString Calculator::createNomineeToalScoreSetString(const QSet<QString>& nominees)
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

QString Calculator::createNomineeRankListString(const QList<Rank>& ranks)
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
std::optional<Calculator::ExtendedTiebreakMethod> Calculator::extraTiebreakMethod() const { return mExtraTiebreakMethod; }
bool Calculator::isExtraTiebreak() const { return mExtraTiebreakMethod.has_value(); }
const Election* Calculator::election() const { return mElection; }
void Calculator::setElection(const Election* election) { mElection = election; }
void Calculator::setExtraTiebreakMethod(std::optional<ExtendedTiebreakMethod> method) { mExtraTiebreakMethod = method; }

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

    // Determine preliminary leaders based on raw score
    QSet<QString> preliminaryLeaders = determinePreliminaryLeaders();

    // Perform primary runoff
    QPair<QSet<QString>, QSet<QString>> results = performPrimaryRunoff(preliminaryLeaders);

    // Check if extended tiebreak could matter
    if(results.first.size() > 1 || results.second.size() > 1)
    {
        // Test outcome with each method (for analysis purposes)
        QHash<ExtendedTiebreakMethod, QPair<QSet<QString>, QSet<QString>>> extendedResults;

        constexpr auto methods = magic_enum::enum_values<ExtendedTiebreakMethod>();
        for(ExtendedTiebreakMethod method : methods)
        {
            emit calculationDetail(LOG_EVENT_EXTENDED_TIEBREAK_EVAL.arg(ENUM_NAME(method)));
            extendedResults[method] = performExtendedTiebreak(results.first, results.second, method);
        }

        if(!mExtraTiebreakMethod.has_value())
            emit calculationDetail(LOG_EVENT_EXTENDED_TIEBREAK_DISABLED);
        else
        {
            ExtendedTiebreakMethod selectedMethod = mExtraTiebreakMethod.value();
            auto selectedMethodResults = extendedResults[selectedMethod];

            if(selectedMethodResults == results)
                emit calculationDetail(LOG_EVENT_EXTENDED_TIEBREAK_IRRELAVENT);
            else
            {
                emit calculationDetail(LOG_EVENT_EXTENDED_TIEBREAK_ENABLED);
                results = selectedMethodResults;
            }
        }
    }
    else
        emit calculationDetail(LOG_EVENT_EXTENDED_TIEBREAK_SKIP);

    // Note final results
    emit calculationDetail(LOG_EVENT_FINAL_RESULT_WINNERS.arg(Qx::String::join(results.first, ", ")));
    emit calculationDetail(LOG_EVENT_FINAL_RESULT_RUNNERUPS.arg(Qx::String::join(results.second, ", ")));

    // Log finish
    emit calculationDetail(LOG_EVENT_CALC_FINISH + '\n' + QString(120,'-'));

    // Return final results
    return ElectionResult(mElection, results.first, results.second);
}


}
