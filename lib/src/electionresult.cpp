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
    mUnresolvedNominees()
{}

ElectionResult::ElectionResult(const Election* election, const QStringList& winners, const QSet<QString> unresolved) :
    mElection(election),
    mWinners(winners),
    mUnresolvedNominees(unresolved)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ElectionResult::isNull() const { return !mElection || (mWinners.isEmpty() && mUnresolvedNominees.isEmpty()); }
bool ElectionResult::isComplete() const { return mUnresolvedNominees.isEmpty(); }
QStringList ElectionResult::winners() const { return mWinners; }
QSet<QString> ElectionResult::unresolvedNominees() const { return mUnresolvedNominees; }
int ElectionResult::filledSeatCount() const { return mWinners.size(); }
int ElectionResult::unfilledSeatCount() const { return mElection ? mElection->seatCount() - mWinners.size() : 0; }
const Election* ElectionResult::election() const { return mElection; }

bool ElectionResult::operator==(const ElectionResult& other) const
{
    return mElection == other.mElection && mWinners == other.mWinners && mUnresolvedNominees == other.mUnresolvedNominees;
}

bool ElectionResult::operator!=(const ElectionResult& other) const { return !(*this == other); }
}
