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

ExpectedElectionResult::ExpectedElectionResult(const QSet<QString>& winners, const QSet<QString>& runnerUps) :
    mWinners(winners),
    mRunnerUps(runnerUps)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ExpectedElectionResult::isNull() const { return mWinners.isEmpty(); }

const QSet<QString>& ExpectedElectionResult::winners() const { return mWinners; }
const QSet<QString>& ExpectedElectionResult::runnerUps() const { return mRunnerUps; }

bool ExpectedElectionResult::operator==(const ElectionResult& result) const
{
    return result.winners() == mWinners && result.runnerUps() == mRunnerUps;
}

}
