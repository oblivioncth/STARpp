// Unit Include
#include "star/rank.h"

// Qt Includes
#include <QMultiMap>

//===============================================================================================================
// Rank
//===============================================================================================================

//-Struct Functions-------------------------------------------------------------------------------------------------
//Public:
QList<Rank> Rank::rankSort(const QMap<QString, int>& valueMap, Order order)
{
    QList<Rank> rankings;

    /* Insert all candidates into a multi-map, keyed by total score. Because maps are naturally sorted
     * by key (ascending), this will create a ranking list from lowest value rank to highest value rank with all
     * candidates at a given rank (> 1 if there are ties) coupled to their corresponding score as the key.
     */
    QMultiMap<int, QStringView> naturalRankings;
    for(auto [candidate, value] : valueMap.asKeyValueRange())
        naturalRankings.insert(value, candidate);

    /* QMultiMap stores multiple values for the same key sequentially, requiring awkward iteration
     * over the map in which one must keep checking if the key has changed since there is no way
     * to iterate by key "group". So, transform the rankings into a more convenient form that
     * uses a list for candidates.
     */
    int currentKey = naturalRankings.firstKey();
    Rank currentRank = {.value = currentKey, .candidates = {}};
    for(auto [totalScore, candidate] : naturalRankings.asKeyValueRange())
    {
        // Check if key/rank has changed
        if(totalScore != currentKey)
        {
            // Finish current rank
            if(order == Descending)
                rankings.prepend(currentRank);
            else
                rankings.append(currentRank);

            // Prepare next rank
            currentRank = {.value = totalScore, .candidates = {}};

            // Update current key
            currentKey = totalScore;
        }

        // Add candidate to rank
        currentRank.candidates.insert(candidate.toString());
    }

    // Finish last rank (necessary since the loop stops before doing this since it uses look-behind)
    if(order == Descending)
        rankings.prepend(currentRank);
    else
        rankings.append(currentRank);

    return rankings;
}
