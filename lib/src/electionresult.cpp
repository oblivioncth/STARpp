// Unit Include
#include "star/electionresult.h"

namespace Star
{

//===============================================================================================================
// ElectionResult
//===============================================================================================================

/*!
 *  @class ElectionResult star/electionresult.h
 *
 *  @brief The ElectionResult class holds the outcome of an election.
 *
 *  Election results consist primarily of a list of seats filled according to the makeup of an Election.
 *
 *  An election result for a given Election can be generated through the use of Calculator.
 *
 *  @sa ExpectedElectionResult, and QualifierResult.
 */

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Constructs a null election result.
 *
 *  @sa isNull().
 */
ElectionResult::ElectionResult() :
    mElection(nullptr),
    mSeats()
{}

/*!
 *  Constructs a election result that corresponds to the election @a election and consists of the evaluated
 *  seats @a seats.
 */
ElectionResult::ElectionResult(const Election* election, const QList<Seat>& seats) :
    mElection(election),
    mSeats(seats)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Returns @c true if the election result is null; otherwise, returns @c false.
 *
 *  @sa isComplete().
 */
bool ElectionResult::isNull() const { return !mElection || mSeats.isEmpty(); }

/*!
 *  Returns @c true if all the seats of the election were able to be filled; otherwise, returns false.
 *
 *  @sa isNull() and Calculator::Option::AllowTrueTies.
 */
bool ElectionResult::isComplete() const { return !isNull() && unfilledSeatCount() == 0; }

/*!
 *  Returns the seat at position @a i.
 */
const Seat& ElectionResult::seatAt(qsizetype i) const
{
    Q_ASSERT_X(size_t(i) < size_t(mSeats.size()), "ElectionResult::at", "index out of range");

    return mSeats.at(i);
}

/*!
 *  Returns the list of seats evaluated for the election.
 */
QList<Seat> ElectionResult::seats() const { return mSeats; }

/*!
 *  Returns a list of the candidates that won each seat.
 */
QStringList ElectionResult::winners() const
{
    QStringList wrs;
    for(const Seat& seat : mSeats)
        if(!seat.winner().isNull()) // Only the last can be null
            wrs.append(seat.winner());

    return wrs;
}

/*!
 *  Returns the set of candidates that caused the election to be ended prematurely, if it is
 *  incomplete; otherwise, returns an empty set.
 *
 *  @sa isComplete() and Calculator::Option::AllowTrueTies.
 */
QSet<QString> ElectionResult::unresolvedCandidates() const
{
    if(isNull() || isComplete())
        return QSet<QString>();
    else
        return mSeats.back().qualifierResult().unresolved();
}

/*!
 *  Returns the seat in the results that was left unfilled, if the election is incomplete;
 *  otherwise, returns a null seat.
 *
 *  If the election is incomplete, this is equivalent to:
 *  @code{.cpp}
 *  seatAt(seatCount() - 1);
 *  @endcode
 *
 *  @sa isComplete() and unresolvedCandidates().
 */
Seat ElectionResult::unresolvedSeat() const
{
    if(isNull() || isComplete())
        return Seat();
    else
        return mSeats.back();
}

/*!
 *  Returns the number of seats that were evaluated.
 *
 *  This will always be equal to the seat count of the election that corresponds with this
 *  result, unless Calculator::Option::AllowTrueTies was set in the Calculator that
 *  generated it and an unresolvable tie occurred.
 *
 *  @sa isComplete(), filledSeatCount() and unfilledSeatCount().
 */
qsizetype ElectionResult::seatCount() const { return mSeats.size(); }

/*!
 *  Returns the number of seats that were able to successfully be filled.
 *
 *  @sa isComplete() and unfilledSeatCount().
 */
qsizetype ElectionResult::filledSeatCount() const
{
    if(isNull())
        return 0;

    qsizetype count = mSeats.size();
    if(mSeats.back().winner().isNull())
        count--;

    return count;
}

/*!
 *  Returns the number of seats that were were left unfilled.
 *
 *  @sa isComplete() and filledSeatCount().
 */
qsizetype ElectionResult::unfilledSeatCount() const { return !isNull() ? mElection->seatCount() - filledSeatCount() : 0; }

/*!
 *  Returns the election that was evaluated.
 */
const Election* ElectionResult::election() const { return mElection; }

/*!
 *  Returns true if this election result is the same as @a other; otherwise, returns false.
 *
 *  @sa operator!=().
 */
bool ElectionResult::operator==(const ElectionResult& other) const
{
    return mElection == other.mElection && mSeats == other.mSeats;
}

/*!
 *  Returns true if this election result is not the same as @a other; otherwise, returns false.
 *
 *  @sa operator==().
 */
bool ElectionResult::operator!=(const ElectionResult& other) const { return !(*this == other); }
}
