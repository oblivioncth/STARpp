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
    mUnresolvedNominees()
{}

ExpectedElectionResult::ExpectedElectionResult(const QStringList& winners, const QSet<QString> unresolved) :
    mWinners(winners),
    mUnresolvedNominees(unresolved)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ExpectedElectionResult::isNull() const { return mWinners.isEmpty(); }
QStringList ExpectedElectionResult::winners() const { return mWinners; }
QSet<QString> ExpectedElectionResult::unresolvedNominees() const { return mUnresolvedNominees; }

void ExpectedElectionResult::setWinners(const QStringList& winner) { mWinners = winner; }
void ExpectedElectionResult::setUnresolvedNominees(QSet<QString> unresolved) { mUnresolvedNominees = unresolved; }

bool ExpectedElectionResult::operator==(const ElectionResult& result) const
{
    return result.winners() == mWinners && result.unresolvedNominees() == mUnresolvedNominees;
}

bool ExpectedElectionResult::operator!=(const ElectionResult& result) const { return !(*this == result); }

}
