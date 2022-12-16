// Unit Include
#include "headtoheadresults.h"

// Project Includes
#include "star/election.h"

namespace Star
{

//===============================================================================================================
// HeadToHeadResults
//===============================================================================================================

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
HeadToHeadResults::HeadToHeadResults(const Election* election)
{
    // Get candidate list
    QStringList candidates = election->candidates();

    // Perform face-offs
    for(auto itrA = candidates.cbegin(); itrA != candidates.cend() - 1; itrA++)
    {
        QString opponentA = *itrA;

        for(auto itrB = itrA + 1; itrB != candidates.cend(); itrB++)
        {
            QString opponentB = *itrB;

            // Determine pref counts
            int prefA = 0;
            int prefB = 0;

            for(const Star::Election::Ballot& ballot : election->ballots())
            {
                QString pref = ballot.preference(opponentA, opponentB);
                if(pref == opponentA)
                    prefA++;
                else if(pref == opponentB)
                    prefB++;
            }

            // Update stats for candidate A
            CandidateStats& statsA = mStats[opponentA];
            if(prefA > prefB)
                statsA.victories.insert(opponentB);
            else if(prefA < prefB)
                statsA.defeats.insert(opponentB);
            statsA.preferences.increase(opponentB, prefA);
            statsA.antiPreferences.increase(opponentB, prefB);

            // Update stats for candidate B
            CandidateStats& statsB = mStats[opponentB];
            if(prefB > prefA)
                statsB.victories.insert(opponentA);
            else if(prefB < prefA)
                statsB.defeats.insert(opponentA);
            statsB.preferences.increase(opponentA, prefB);
            statsB.antiPreferences.increase(opponentA, prefA);
        }
    }
}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
int HeadToHeadResults::wins(const QString& candidate) const
{
    if(!mStats.contains(candidate))
        return 0;

    return mStats[candidate].victories.size();
}

int HeadToHeadResults::losses(const QString& candidate) const
{
    if(!mStats.contains(candidate))
        return 0;

    return mStats[candidate].defeats.size();
}

int HeadToHeadResults::preferences(const QString& candidate) const
{
    if(!mStats.contains(candidate))
        return 0;

    return mStats[candidate].preferences.total();
}

int HeadToHeadResults::margin(const QString& candidate) const
{
    if(!mStats.contains(candidate))
        return 0;

    auto stat = mStats[candidate];
    return stat.preferences.total() - stat.antiPreferences.total();
}

QString HeadToHeadResults::winner(const QString& candidateA, const QString& candidateB) const
{
    if(!mStats.contains(candidateA) || !mStats.contains(candidateB))
        return QString();

    return mStats[candidateA].victories.contains(candidateB) ? candidateA :
           mStats[candidateB].victories.contains(candidateA) ? candidateB :
                                                               QString();
}

qsizetype HeadToHeadResults::candidateCount() const { return mStats.size(); }

QSet<QString> HeadToHeadResults::candidates() const
{
    QSet<QString> can;
    for(auto itr = mStats.cbegin(); itr != mStats.cend(); itr++)
        can.insert(itr.key());
    return can;
}

void HeadToHeadResults::narrow(QSet<QString> candidates, NarrowMode mode)
{
    auto shouldCull = [&](const QString& c){
        return candidates.contains(c) && mode == Exclusive ||
              !candidates.contains(c) && mode == Inclusive;
    };

    auto sItr = mStats.begin();
    while(sItr != mStats.end())
    {
        if(shouldCull(sItr.key()))
            sItr = mStats.erase(sItr);
         else
            sItr++;
    }

    for(CandidateStats& stat : mStats)
    {
        stat.defeats.removeIf([&](const QString& c){ return shouldCull(c); });
        stat.victories.removeIf([&](const QString& c) { return shouldCull(c); });

        // NOTE: Getting list from preferences is fine because the list should always be the same for pref and anti-pref
        Q_ASSERT(stat.preferences.count() == stat.antiPreferences.count());
        QList<QString> cans = stat.preferences.components();
        for(const QString& c : cans)
        {
            if(shouldCull(c))
            {
                stat.preferences.remove(c);
                stat.antiPreferences.remove(c);
            }
        }
    }
}

HeadToHeadResults HeadToHeadResults::narrowed(QSet<QString> candidates, NarrowMode mode)
{
    HeadToHeadResults cpy(*this);
    cpy.narrow(candidates, mode);
    return cpy;
}

}
