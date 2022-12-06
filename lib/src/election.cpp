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
bool Election::isValid() const
{
    return nominees().count() > 1 && ballots().count() > 1 && seatCount() > 0 && seatCount() <= nominees().count();
}

QString Election::name() const { return mName; }
QStringList Election::nominees() const { return mTotals.keys(); }
const QList<Election::Ballot>& Election::ballots() const { return mBallots; }
int Election::seatCount() const { return mSeats; }

int Election::totalScore(const QString& nominee) const
{
    // NOTE: This could instead return a std::optional<int> for if *nominee* is missing, but that
    // would indicate a bug elsewhere and should never happen, so instead this just throws.
    if(!mTotals.contains(nominee))
        qFatal(" the desired nominee is not present.");

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
int Election::Ballot::score(const QString& nominee) const { return mVotes.value(nominee, 0); }

QString Election::Ballot::preference(const QString& nomineeA, const QString& nomineeB) const
{
    int scoreA = score(nomineeA);
    int scoreB = score(nomineeB);

    return scoreA > scoreB ? nomineeA :
           scoreB > scoreA ? nomineeB :
           QString();
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

        int score = std::max(0, std::min(vote.score, 5));
        ballot.mVotes[nominee] = score;
        mConstruct.mTotals[nominee] += score;
    }

    // Add ballot to construct
    mConstruct.mBallots.append(ballot);

    return *this;
}

Election::Builder& Election::Builder::wSeatCount(int count) { mConstruct.mSeats = count; return *this; }

void Election::Builder::reset()
{
    QString name = mConstruct.mName;
    mConstruct = Election();
    mConstruct.mName = name;
}

Election Election::Builder::build()
{
    // Form rankings
    mConstruct.mScoreRankings = Rank::rankSort(mConstruct.mTotals);

    // Return completed construct
    return mConstruct;
}

}
