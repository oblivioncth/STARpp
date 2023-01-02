// Unit Include
#include "star/rank.h"

// Qt Includes
#include <QMultiMap>

//===============================================================================================================
// Rank
//===============================================================================================================

/*!
 *  @struct Rank star/rank.h
 *
 *  @brief The Rank struct represents a relative position for a given standing that can be occupied by multiple
 *  candidates.
 *
 *  This struct is generally used within a list in order to present overall candidate rankings for different
 *  metrics.
 */

//-Struct Enums---------------------------------------------------------------------------------------------------
//Public:
/*!
 *  @enum Rank::Order
 *
 *  This enum represents the sort order of rankings.
 */

/*!
 *  @var Rank::Order Rank::Ascending
 *  Ascending sort order.
 */

/*!
 *  @var Rank::Order Rank::Descending
 *  Descending sort order.
 */

//-Struct Fields---------------------------------------------------------------------------------------------------
//Public:
/*!
 *  @var int Rank::value
 *
 *  The value that defines the rank.
 */

/*!
 *  @var QSet<QString> Rank::candidates
 *
 *  The candidates that are part of the rank.
 */

//-Struct Functions-------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Creates rankings of the candidates in the candidate-score map @a valueMap, sorted
 *  according to @a order.
 */
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
