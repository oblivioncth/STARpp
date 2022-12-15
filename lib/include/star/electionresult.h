#ifndef ELECTIONRESULT_H
#define ELECTIONRESULT_H

// Qt Includes

// Project Includes
#include "star/election.h"

namespace Star
{

class ElectionResult
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    const Election* mElection;
    QStringList mWinners;
    QSet<QString> mUnresolvedCandidates;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ElectionResult();
    ElectionResult(const Election* election, const QStringList& winners, const QSet<QString> unresolved);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;
    bool isComplete() const;
    QStringList winners() const;
    QSet<QString> unresolvedCandidates() const;
    int filledSeatCount() const;
    int unfilledSeatCount() const;
    const Election* election() const;

    bool operator==(const ElectionResult& other) const;
    bool operator!=(const ElectionResult& other) const;
};

}

#endif // ELECTIONRESULT_H
