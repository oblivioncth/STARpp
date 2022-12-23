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
    return candidates().count() > 1 && ballots().count() > 1 && seatCount() > 0 && seatCount() <= candidates().count();
}

QString Election::name() const { return mName; }
QStringList Election::candidates() const { return mTotals.keys(); }
const QList<Election::Ballot>& Election::ballots() const { return mBallots; }
int Election::seatCount() const { return mSeats; }

int Election::totalScore(const QString& candidate) const
{
    if(!mTotals.contains(candidate))
        qWarning("the desired candidate is not present.");

    return mTotals.value(candidate, 0);
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
int Election::Ballot::score(const QString& candidate) const { return mVotes.value(candidate, 0); }

QString Election::Ballot::preference(const QString& candidateA, const QString& candidateB) const
{
    int scoreA = score(candidateA);
    int scoreB = score(candidateB);

    return scoreA > scoreB ? candidateA :
           scoreB > scoreA ? candidateB :
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
        const QString& candidate = vote.candidate;

        int score = std::max(0, std::min(vote.score, 5));
        ballot.mVotes[candidate] = score;
        mConstruct.mTotals[candidate] += score;
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
