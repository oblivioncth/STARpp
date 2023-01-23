// Unit Include
#include "star/election.h"

namespace Star
{

//===============================================================================================================
// Election
//===============================================================================================================

/*!
 *  @class Election star/election.h
 *
 *  @brief The Election class represents a collection of candidates and ballots.
 *
 *  One of the two quintessential classes of STAR++, the Election class acts as a literal election. An Election
 *  is created via Election::Builder and then "counted" with a Calculator, which provides a corresponding
 *  ElectionResults instance.
 *
 *  @sa Calculator.
 */

//-Constructor--------------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Creates an invalid Election.
 *
 *  @sa Election::Builder.
 */
Election::Election() {}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Returns @c true if the election is valid; otherwise, returns false.
 *
 *  In order for an election to be valid, it must:
 *  @li Have two or more candidates
 *  @li Have two or more ballots
 *  @li Have a seat count of one or greater
 *  @li Have a seat count that is less than or equal to its number of candidates
 */
bool Election::isValid() const
{
    return candidates().count() > 1 && ballots().count() > 1 && seatCount() > 0 && seatCount() <= candidates().count();
}

/*!
 *  Returns the name of the election.
 */
QString Election::name() const { return mName; }

/*!
 *  Returns the list of candidates in the election.
 */
QStringList Election::candidates() const { return mTotals.keys(); }

/*!
 *  Returns the list of ballots provided for the election.
 */
const QList<Election::Ballot>& Election::ballots() const { return mBallots; }

/*!
 *  Returns the number of seats prescribed for the election.
 */
int Election::seatCount() const { return mSeats; }

/*!
 * Returns the total score for candidate @a candidate across all ballots.
 */
int Election::totalScore(const QString& candidate) const
{
    if(!mTotals.contains(candidate))
        qWarning("the desired candidate is not present.");

    return mTotals.value(candidate, 0);
}

/*!
 *  Returns a list of all candidates in the election ranked by total score (descending).
 */
const QList<Rank>& Election::scoreRankings() const { return mScoreRankings; }

//===============================================================================================================
// Election::Voter
//===============================================================================================================

/*!
 *  @struct Election::Voter star/election.h
 *
 *  @brief The Election::Voter struct contains the details of a voter participating in an election.
 *
 *  @sa Election::Vote.
 */

/*!
 *  @var QString Election::Voter::name
 *
 *  The name of the voter.
 *
 *  @note This field is currently unused.
 */

/*!
 *  @var QString Election::Voter::anonymousName
 *
 *  The anonymized name of the voter. Primarily used for logging.
 */

//===============================================================================================================
// Election::Vote
//===============================================================================================================

/*!
 *  @struct Election::Vote star/election.h
 *
 *  @brief The Election::Voter struct represents a single vote for a candidate in an election.
 *
 *  @sa Election::Voter.
 */

/*!
 *  @var QString Election::Vote::candidate
 *
 *  The candidate the vote is for.
 */

/*!
 *  @var QString Election::Vote::score
 *
 *  The score of the vote.
 */

//===============================================================================================================
// Election::Ballot
//===============================================================================================================

/*!
 *  @class Election::Ballot star/election.h
 *
 *  @brief The Election::Ballot class represents a single ballot for a given election.
 *
 *  A Ballot is composed of a voter and their votes.
 *
 *  Ballots are not designed to be created directly, but rather through Election::Builder::wBallot().
 *
 *  @sa Election::Vote and Election::Voter.
 */

//-Constructor--------------------------------------------------------------------------------------------------------
//Private:
Election::Ballot::Ballot() {}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Returns the voter that the ballot is attributed it.
 */
const Election::Voter& Election::Ballot::voter() const { return mVoter; }

/*!
 *  Returns the score given to @a candidate on the ballot.
 */
int Election::Ballot::score(const QString& candidate) const { return mVotes.value(candidate, 0); }

/*!
 *  Returns the candidate preferred between @a candidateA and @a candidateB on the ballot, or a
 *  null string if neither were preferred.
 */
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

/*!
 *  @class Election::Builder star/election.h
 *
 *  @brief The Election::Builder class is a utility through which elections can be prepared.
 *
 *  Elections are built by first instantiating a Builder, then adding all ballots from participating voters
 *  to it, and finally calling build().
 *
 *  The total pool of candidates is determined automatically while evaluating each Ballot. If a ballot is
 *  missing votes for a candidate that is present in other ballots, that ballots vote for that candidate
 *  is considered to be @c 0.
 *
 *  @sa Election.
 */

//-Constructor--------------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Creates an election builder, poised to create an election named @a name.
 *
 *  @sa build().
 */
Election::Builder::Builder(const QString& name)
{
    mConstruct.mName = name;
}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Creates a ballot containing the @a votes from @a voter and adds them to the builder.
 *
 *  Returns a reference to the builder.
 */
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

/*!
 *  Sets the seat name of the work-in-progress election to @a name.
 *
 *  Returns a reference to the builder.
 */
Election::Builder& Election::Builder::wName(const QString& name) { mConstruct.mName = name; return *this; }

/*!
 *  Sets the seat count of the work-in-progress election to @a count.
 *
 *  Returns a reference to the builder.
 */
Election::Builder& Election::Builder::wSeatCount(int count) { mConstruct.mSeats = count; return *this; }

/*!
 *  Resets the work-in-progress election to a default-constructed one.
 */
void Election::Builder::reset()
{
    QString name = mConstruct.mName;
    mConstruct = Election();
    mConstruct.mName = name;
}

/*!
 *  Completes the work-in-progress election and returns it.
 */
Election Election::Builder::build()
{
    // Form rankings
    mConstruct.mScoreRankings = Rank::rankSort(mConstruct.mTotals);

    // Return completed construct
    return mConstruct;
}

}
