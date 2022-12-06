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
    mWinners(),
    mUnresolvedCandidates()
{}

ElectionResult::ElectionResult(const Election* election, const QStringList& winners, const QSet<QString> unresolved) :
    mElection(election),
    mWinners(winners),
    mUnresolvedCandidates(unresolved)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ElectionResult::isNull() const { return !mElection || (mWinners.isEmpty() && mUnresolvedCandidates.isEmpty()); }
bool ElectionResult::isComplete() const { return mUnresolvedCandidates.isEmpty(); }
QStringList ElectionResult::winners() const { return mWinners; }
QSet<QString> ElectionResult::unresolvedCandidates() const { return mUnresolvedCandidates; }
int ElectionResult::filledSeatCount() const { return mWinners.size(); }
int ElectionResult::unfilledSeatCount() const { return mElection ? mElection->seatCount() - mWinners.size() : 0; }
const Election* ElectionResult::election() const { return mElection; }

bool ElectionResult::operator==(const ElectionResult& other) const
{
    return mElection == other.mElection && mWinners == other.mWinners && mUnresolvedCandidates == other.mUnresolvedCandidates;
}

bool ElectionResult::operator!=(const ElectionResult& other) const { return !(*this == other); }
}
