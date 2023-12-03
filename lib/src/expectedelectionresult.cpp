// Unit Include
#include "star/expectedelectionresult.h"

namespace Star
{

//===============================================================================================================
// ExpectedElectionResult
//===============================================================================================================

/*!
 *  @class ExpectedElectionResult star/expectedelectionresult.h
 *
 *  @brief The ExpectedElectionResult class compares the results of an election to a known outcome.
 *
 *  This class is primarily useful for testing purposes.
 *
 *  @sa ElectionResult.
 */

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Constructs a null expected election result.
 *
 *  @sa ExpectedElectionResult::Builder and isNull().
 */
ExpectedElectionResult::ExpectedElectionResult() :
    mSeats()
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Returns @c true if the expected election result is null; otherwise, returns @a false.
 */
bool ExpectedElectionResult::isNull() const { return mSeats.isEmpty(); }

/*!
 *  Returns @c true if the seat winners of this expected election result are the same as in the
 *  election result @a result; otherwise, returns @c false.
 *
 *  This method ignores any potential qualifier round differences between the results and only
 *  compares the final seat winners.
 */
bool ExpectedElectionResult::sameOutcomeAs(const ElectionResult& result)
{
    if(mSeats.size() != result.seatCount())
        return false;
    else
    {
        for(auto s = 0; s != mSeats.size(); s++)
            if(mSeats.at(s).winner() != result.seatAt(s).winner())
                return false;

        return true;
    }
}

/*!
 *  Returns the number of seats that are expected to have been evaluated.
 *
 *  @sa seats().
 */
qsizetype ExpectedElectionResult::seatCount() const { return mSeats.size(); }

/*!
 *  Returns the list of expected evaluated seats.
 *
 *  @sa seatCount().
 */
QList<Seat> ExpectedElectionResult::seats() const { return mSeats; }

/*!
 *  Returns @c true if this expected election result is the same as the election result
 *  @a result; otherwise, returns @c false.
 *
 *  @sa sameOutcomeAs().
 */
bool ExpectedElectionResult::operator==(const ElectionResult& result) const
{
    return mSeats == result.seats();
}

//===============================================================================================================
// ExpectedElectionResult::Builder
//===============================================================================================================

/*!
 *  @class ExpectedElectionResult::Builder star/expectedelectionresult.h
 *
 *  @brief The ExpectedElectionResult::Builder class is a utility through expected election result
 *  sets can be created.
 *
 *  ExpectedElectionResults are built by first instantiating a Builder, then adding all predetermined
 *  seats to it, and finally calling build().
 */

//-Constructor--------------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Creates an expected election result builder.
 *
 *  @sa build().
 */
ExpectedElectionResult::Builder::Builder() {}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Adds the seat @a seat to the builder.
 *
 *  @sa wWinner().
 */
ExpectedElectionResult::Builder& ExpectedElectionResult::Builder::wSeat(const Seat& seat)
{
    mConstruct.mSeats.append(seat); return *this;
}

/*!
 *  Adds a seat to the builder with winner @a winner and a null QualifierResult.
 *
 *  This is a convenience function that can be used along with sameOutcomeAs() to check election results
 *  when you only care about the winners of each seat.
 *
 *  @sa wSeat().
 */
ExpectedElectionResult::Builder& ExpectedElectionResult::Builder::wWinner(const QString& winner)
{
    mConstruct.mSeats.append(Seat(winner)); return *this;
}

/*!
 *  Completes the work-in-progress expected election and returns it.
 */
ExpectedElectionResult ExpectedElectionResult::Builder::build()
{
    // Return completed construct
    return mConstruct;
}

}
