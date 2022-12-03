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

    /* Insert all nominees into a multi-map, keyed by total score. Because maps are naturally sorted
     * key (ascending), this will create a ranking list from lowest rank to highest rank with all
     * nominees at a given rank (> 1 if there are ties) tied to its corresponding score as the key.
     */
    QMultiMap<int, QStringView> reverseRankings;
    for(auto [nominee, value] : valueMap.asKeyValueRange())
        reverseRankings.insert(value, nominee);

    /* QMultiMap stores multiple values for the same key sequentially, requiring awkward iteration
     * over the map in which one must keep checking if the key has changed since there is no way
     * to iterate by key "group". So, transform the rankings into a more convenient form that
     * uses a list for nominees.
     */
    int currentKey = reverseRankings.firstKey();
    Rank currentRank = {.value = currentKey, .nominees = {}};
    for(auto [totalScore, nominee] : reverseRankings.asKeyValueRange())
    {
        // Check if key/rank has changed
        if(totalScore != currentKey)
        {
            // Finish current rank
            if(order == Ascending)
                rankings.prepend(currentRank);
            else
                rankings.append(currentRank);

            // Prepare next rank
            currentRank = {.value = totalScore, .nominees = {}};

            // Update current key
            currentKey = totalScore;
        }

        // Add nominee to rank
        currentRank.nominees.insert(nominee.toString());
    }

    // Finish last rank (necessary since the loop stops before doing this since it uses look-behind)
    rankings.prepend(currentRank);

    return rankings;
}
