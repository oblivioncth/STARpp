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
    mUnresolvedCandidates()
{}

ExpectedElectionResult::ExpectedElectionResult(const QStringList& winners, const QSet<QString> unresolved) :
    mWinners(winners),
    mUnresolvedCandidates(unresolved)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool ExpectedElectionResult::isNull() const { return mWinners.isEmpty(); }
QStringList ExpectedElectionResult::winners() const { return mWinners; }
QSet<QString> ExpectedElectionResult::unresolvedCandidates() const { return mUnresolvedCandidates; }

void ExpectedElectionResult::setWinners(const QStringList& winner) { mWinners = winner; }
void ExpectedElectionResult::setUnresolvedCandidates(QSet<QString> unresolved) { mUnresolvedCandidates = unresolved; }

bool ExpectedElectionResult::operator==(const ElectionResult& result) const
{
    return result.winners() == mWinners && result.unresolvedCandidates() == mUnresolvedCandidates;
}

bool ExpectedElectionResult::operator!=(const ElectionResult& result) const { return !(*this == result); }

}
