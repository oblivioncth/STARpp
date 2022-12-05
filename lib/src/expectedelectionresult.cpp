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
    mWinner()
{}

ExpectedElectionResult::ExpectedElectionResult(const QString& winner) :
    mWinner(winner)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ExpectedElectionResult::isNull() const { return mWinner.isEmpty(); }

QString ExpectedElectionResult::winner() const { return mWinner; }

bool ExpectedElectionResult::operator==(const ElectionResult& result) const
{
    return result.winner() == mWinner;
}

}
