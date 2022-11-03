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

ElectionResult::ElectionResult(const Election* election, const QSet<QString>& winners, const QSet<QString>& runnerUps) :
    mElection(election),
    mWinners(winners),
    mRunnerUps(runnerUps)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ElectionResult::isNull() const { return !mElection || mWinners.isEmpty(); }

const Election* ElectionResult::election() const { return mElection; }
const QSet<QString>& ElectionResult::winners() const { return mWinners; }
const QSet<QString>& ElectionResult::runnerUps() const { return mRunnerUps; }



}
