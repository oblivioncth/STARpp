// Unit Include
#include "starcalc/election.h"

//===============================================================================================================
// Category
//===============================================================================================================

//-Constructor--------------------------------------------------------------------------------------------------------
//Public:
Election::Election() {}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
QString Election::name() const { return mName; }
QStringList Election::nominees() const { return mTotals.keys(); }
const QList<Election::Ballot>& Election::ballots() const { return mBallots; }

uint Election::totalScore(const QString& nominee) const
{
    // NOTE: This could instead return a std::optional<uint> for if *nominee* is missing, but that
    // would indicate a bug elsewhere and should never happen, so instead this just throws.
    if(!mTotals.contains(nominee))
        throw std::runtime_error(std::string(Q_FUNC_INFO) + " the desired nominee is not present.");

    return mTotals[nominee];
}

const QList<Election::Rank>& Election::rankings() const { return mRankings; }

//===============================================================================================================
// Category::Ballot
//===============================================================================================================

//-Constructor--------------------------------------------------------------------------------------------------------
//Private:
Election::Ballot::Ballot() {}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
const Voter& Election::Ballot::voter() const { return mVoter; }
uint Election::Ballot::score(const QString& nominee) const { return mVotes.value(nominee, 0); }

QString Election::Ballot::preference(const QString& first, const QString& second) const
{
    uint firstPoints = score(first);
    uint secondPoints = score(second);

    if(firstPoints == secondPoints)
        return QString();
    else if(firstPoints > secondPoints)
        return first;
    else
        return second;
}

//===============================================================================================================
// Category::Builder
//===============================================================================================================

//-Constructor--------------------------------------------------------------------------------------------------------
//Public:
Election::Builder::Builder(const QString& name)
{
    mConstruct.mName = name;
}

//-Class Functions-------------------------------------------------------------------------------------------------
//Private:
QList<Election::Rank> Election::Builder::formRankings(const QMap<QString, uint>& totalScores)
{
    QList<Rank> rankings;

    /* Insert all nominees into a multi-map, keyed by total score. Because maps are naturally sorted
     * key (ascending), this will create a ranking list from lowest rank to highest rank with all
     * nominees at a given rank (> 1 if there are ties) tied to its corresponding score as the key.
     */
    QMultiMap<uint, QStringView> reverseRankings;
    for(auto [nominee, totalScore] : totalScores.asKeyValueRange())
        reverseRankings.insert(totalScore, nominee);

    /* QMultiMap stores multiple values for the same key sequentially, requiring awkward iteration
     * over the map in which one must keep checking if the key has changed since there is no way
     * to iterate by key "group". So, transform the rankings into a more convenient form that
     * uses a list for nominees.
     */
    uint currentKey = reverseRankings.firstKey();
    Rank currentRank = {.totalScore = currentKey, .nominees = {}};
    for(auto [totalScore, nominee] : reverseRankings.asKeyValueRange())
    {
        // Check if key/rank has changed
        if(totalScore != currentKey)
        {
            // Finish current rank
            rankings.prepend(currentRank); // Prepend to flip the order so that index '0' is 1st place

            // Prepare next rank
            currentRank = {.totalScore = totalScore, .nominees = {}};

            // Update current key
            currentKey = totalScore;
        }

        // Add nominee to rank
        currentRank.nominees.append(nominee.toString());
    }

    // Finish last rank (necessary since the loop stops before doing this since it uses look-behind)
    rankings.prepend(currentRank);

    return rankings;
}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
Election::Builder& Election::Builder::wBallot(const Voter& voter, const QList<Vote>& votes)
{
    // Create ballot and add scores to construct total tracker
    Election::Ballot ballot;
    ballot.mVoter = voter;

    for(const Vote& vote : votes)
    {
        const QString& nominee = vote.nominee;
        uint score = vote.score;
        ballot.mVotes[nominee] = score;
        mConstruct.mTotals[nominee] += score;
    }

    // Add ballot to construct
    mConstruct.mBallots.append(ballot);

    return *this;
}

Election Election::Builder::build()
{
    // Form rankings
    mConstruct.mRankings = formRankings(mConstruct.mTotals);

    // Return completed construct
    return mConstruct;
}
