// Unit Include
#include "star/expectedelectionresult.h"

namespace Star
{

//===============================================================================================================
// ExpectedElectionResult
//===============================================================================================================

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
ExpectedElectionResult::ExpectedElectionResult() :
    mSeats()
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ExpectedElectionResult::isNull() const { return mSeats.isEmpty(); }

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

qsizetype ExpectedElectionResult::seatCount() const { return mSeats.size(); }
QList<Seat> ExpectedElectionResult::seats() const { return mSeats; }

bool ExpectedElectionResult::operator==(const ElectionResult& result) const
{
    return mSeats == result.seats();
}

bool ExpectedElectionResult::operator!=(const ElectionResult& result) const { return !(*this == result); }

//===============================================================================================================
// ExpectedElectionResult::Builder
//===============================================================================================================

//-Constructor--------------------------------------------------------------------------------------------------------
//Public:
ExpectedElectionResult::Builder::Builder() {}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
ExpectedElectionResult::Builder& ExpectedElectionResult::Builder::wSeat(const Seat& seat)
{
    mConstruct.mSeats.append(seat); return *this;
}

ExpectedElectionResult::Builder& ExpectedElectionResult::Builder::wWinner(const QString& winner)
{
    mConstruct.mSeats.append(Seat(winner)); return *this;
}

ExpectedElectionResult ExpectedElectionResult::Builder::build()
{
    // Return completed construct
    return mConstruct;
}

}
