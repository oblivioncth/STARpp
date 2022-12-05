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
    mUnresolvedNominees()
{}

ElectionResult::ElectionResult(const Election* election, const QString& winner, const QSet<QString> unresolved) :
    mElection(election),
    mWinner(winner),
    mUnresolvedNominees(unresolved)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ElectionResult::isNull() const { return !mElection || (mWinner.isNull() && mUnresolvedNominees.isEmpty()); }
bool ElectionResult::isComplete() const { return mUnresolvedNominees.isEmpty(); }
QString ElectionResult::winner() const { return mWinner; }
QSet<QString> ElectionResult::unresolvedNominees() const { return mUnresolvedNominees; }
const Election* ElectionResult::election() const { return mElection; }

}
