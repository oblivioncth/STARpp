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
    mWinner()
{}

ElectionResult::ElectionResult(const Election* election, const QString& winner) :
    mElection(election),
    mWinner(winner)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ElectionResult::isNull() const { return !mElection || mWinner.isNull(); }

const Election* ElectionResult::election() const { return mElection; }
QString ElectionResult::winner() const { return mWinner; }



}
