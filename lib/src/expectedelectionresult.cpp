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
    mWinner(),
    mUnresolvedNominees()
{}

ExpectedElectionResult::ExpectedElectionResult(const QString& winner, const QSet<QString> unresolved) :
    mWinner(winner),
    mUnresolvedNominees(unresolved)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ExpectedElectionResult::isNull() const { return mWinner.isEmpty(); }
QString ExpectedElectionResult::winner() const { return mWinner; }
QSet<QString> ExpectedElectionResult::unresolvedNominees() const { return mUnresolvedNominees; }

void ExpectedElectionResult::setWinner(const QString& winner) { mWinner = winner; }
void ExpectedElectionResult::setUnresolvedNominees(QSet<QString> unresolved) { mUnresolvedNominees = unresolved; }

bool ExpectedElectionResult::operator==(const ElectionResult& result) const
{
    return result.winner() == mWinner && result.unresolvedNominees() == mUnresolvedNominees;
}

}
