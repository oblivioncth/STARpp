// Unit Include
#include "star/election.h"

namespace Star
{

//===============================================================================================================
// Election
//===============================================================================================================

//-Constructor--------------------------------------------------------------------------------------------------------
//Public:
Election::Election() {}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool Election::isValid() const { return nominees().count() > 1 && ballots().count() > 1; }

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

const QList<Rank>& Election::scoreRankings() const { return mScoreRankings; }

//===============================================================================================================
// Election::Ballot
//===============================================================================================================

//-Constructor--------------------------------------------------------------------------------------------------------
//Private:
Election::Ballot::Ballot() {}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
const Election::Voter& Election::Ballot::voter() const { return mVoter; }
uint Election::Ballot::score(const QString& nominee) const { return mVotes.value(nominee, 0); }

QString Election::Ballot::preference(const QStringList& nominees) const
{
    QString pref;
    uint prefScore = 0;

    // Check for highest score
    for(const QString& nominee : nominees)
    {
        uint nomScore = score(nominee);
        if(nomScore == prefScore)
        {
            pref = QString(); // Score ties prevent preference
            if(nomScore == 5) // A score tie of 5 means a preference is impossible
                break;
        }
        else if(nomScore > prefScore)
        {
            pref = nominee;
            prefScore = nomScore;
        }
    }

    return pref;
}

//===============================================================================================================
// Election::Builder
//===============================================================================================================

//-Constructor--------------------------------------------------------------------------------------------------------
//Public:
Election::Builder::Builder(const QString& name)
{
    mConstruct.mName = name;
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
    mConstruct.mScoreRankings = Rank::rankSort(mConstruct.mTotals);

    // Return completed construct
    return mConstruct;
}

}
