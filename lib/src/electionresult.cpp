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
    mWinner(),
    mRunnerUp(),
    mTie(false)
{}

ElectionResult::ElectionResult(const Election* election, const QString& winner, const QString& runnerUp, bool tie) :
    mElection(election),
    mWinner(winner),
    mRunnerUp(runnerUp),
    mTie(tie)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ElectionResult::isNull() const { return !mElection; }

const Election* ElectionResult::election() const { return mElection; }
QString ElectionResult::winner() const { return mWinner; }
QString ElectionResult::runnerUp() const { return mRunnerUp; }
bool ElectionResult::isTie() const { return mTie; }

}
