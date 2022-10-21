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
    mRunnerUps()
{}

ElectionResult::ElectionResult(const Election* election, const QStringList& winners, const QStringList& runnerUps) :
    mElection(election),
    mWinners(winners),
    mRunnerUps(runnerUps)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ElectionResult::isNull() const { return !mElection || mWinners.isEmpty(); }

const Election* ElectionResult::election() const { return mElection; }
const QStringList& ElectionResult::winners() const { return mWinners; }
const QStringList& ElectionResult::runnerUps() const { return mRunnerUps; }



}
