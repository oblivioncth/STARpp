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
    mWinners(),
    mRunnerUps()
{}

ExpectedElectionResult::ExpectedElectionResult(const QStringList& winners, const QStringList& runnerUps) :
    mWinners(winners),
    mRunnerUps(runnerUps)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ExpectedElectionResult::isNull() const { return mWinners.isEmpty(); }

const QStringList& ExpectedElectionResult::winners() const { return mWinners; }
const QStringList& ExpectedElectionResult::runnerUps() const { return mRunnerUps; }

bool ExpectedElectionResult::operator==(const ElectionResult& result) const
{
    return result.winners() == mWinners && result.runnerUps() == mRunnerUps;
}

}
