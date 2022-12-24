// Unit Include
#include "star/seat.h"

namespace Star
{

//===============================================================================================================
// Seat
//===============================================================================================================

/*!
 *  @class Seat star/seat.h
 *
 *  @brief The Seat class contains the outcome for a specific seat in a given election.
 *
 *  A seat consists of a winner and QualifierResult that details how that winner and their opponent were
 *  seeded into the runoff phase of the election they belong to. If a seat could not be filled, it will
 *  contain no winner and its QualifierResult will contain the unresolvable tie that prevented its completion
 *  if it occurred during the scoring round. Otherwise the tie was between the candidates in the runoff.
 *
 *  @sa Calculator::Option::AllowTrueTies.
 *
 *  @sa Election.
 */

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Constructs a null seat.
 *
 *  @sa isNull().
 */
Seat::Seat() :
    mWinner(),
    mQualifierResult()
{}

/*!
 *  Constructs an unfilled seat with qualifier result @a qualifierResult.
 *
 *  @sa isFilled().
 */
Seat::Seat(const QualifierResult& qualifierResult) :
    mWinner(),
    mQualifierResult(qualifierResult)
{}

/*!
 *  Constructs a seat with the winner @a winner and qualifier result @a qualifierResult.
 */
Seat::Seat(const QString& winner, const QualifierResult& qualifierResult) :
    mWinner(winner),
    mQualifierResult(qualifierResult)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Returns @c true if the seat is null; otherwise, returns @c false.
 */
bool Seat::isNull() const { return mWinner.isNull() && mQualifierResult.isNull(); }

/*!
 *  Returns @c true if the seat was successfully filled; otherwise, returns @c false.
 *
 *  @sa Election::isComplete(), winner(), and Calculator::Option::AllowTrueTies.
 */
bool Seat::isFilled() const { return !mWinner.isNull(); }

/*!
 *  Returns @c true if the seat was filled via special exception; otherwise, returns @c false.
 *
 *  Exceptions to the normal seat filling process include:
 *  @li Filling the last seat when only one candidate remains
 *  @li Filling a seat due to Calculator::Option::DefactoWinner
 *
 *  If this is the case, the seat's qualifier result will be null.
 */
bool Seat::isExceptionFilled() const { return !isNull() && mQualifierResult.isNull(); } // NOTE: Filled without runoff

/*!
 *  Returns the candidate that won the seat.
 *
 *  @sa isFilled() and qualifierResult().
 */
QString Seat::winner() const { return mWinner; }

/*!
 *  Returns the qualifier result of the runoff qualifier that was performed while filling the seat.
 *
 *  @sa isFilled() and winner().
 */
QualifierResult Seat::qualifierResult() const { return mQualifierResult; }

/*!
 *  Returns true if this seat is the same as @a other; otherwise, returns false.
 *
 *  @sa operator!=().
 */
bool Seat::operator==(const Seat& other) const
{
    return mWinner == other.winner() && mQualifierResult == other.qualifierResult();
}

/*!
 *  Returns true if this seat is not the same as @a other; otherwise, returns false.
 *
 *  @sa operator==().
 */
bool Seat::operator!=(const Seat& other) const { return !(*this == other); }

}
