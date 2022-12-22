// Unit Include
#include "star/electionresult.h"

namespace Star
{

//===============================================================================================================
// ElectionResult
//===============================================================================================================

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
ElectionResult::ElectionResult() :
    mElection(nullptr),
    mSeats()
{}

ElectionResult::ElectionResult(const Election* election, const QList<Seat>& seats) :
    mElection(election),
    mSeats(seats)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ElectionResult::isNull() const { return !mElection || mSeats.isEmpty(); }
bool ElectionResult::isComplete() const { return !isNull() && unfilledSeatCount() == 0; }

const Seat& ElectionResult::seatAt(qsizetype i) const
{
    Q_ASSERT_X(size_t(i) < size_t(mSeats.size()), "ElectionResult::at", "index out of range");

    return mSeats.at(i);
}

QList<Seat> ElectionResult::seats() const { return mSeats; }

QStringList ElectionResult::winners() const
{
    QStringList wrs;
    for(const Seat& seat : mSeats)
        if(!seat.winner().isNull()) // Only the last can be null
            wrs.append(seat.winner());

    return wrs;
}

QSet<QString> ElectionResult::unresolvedCandidates() const
{
    if(isNull() || isComplete())
        return QSet<QString>();
    else
        return mSeats.back().qualifierResult().unresolved();
}

qsizetype ElectionResult::seatCount() const { return mSeats.size(); }

qsizetype ElectionResult::filledSeatCount() const
{
    if(isNull())
        return 0;

    int count = mSeats.size();
    if(mSeats.back().winner().isNull())
        count--;

    return count;
}

qsizetype ElectionResult::unfilledSeatCount() const { return !isNull() ? mElection->seatCount() - filledSeatCount() : 0; }

const Election* ElectionResult::election() const { return mElection; }

bool ElectionResult::operator==(const ElectionResult& other) const
{
    return mElection == other.mElection && mSeats == other.mSeats;
}

bool ElectionResult::operator!=(const ElectionResult& other) const { return !(*this == other); }
}
