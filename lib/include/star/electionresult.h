#ifndef ELECTIONRESULT_H
#define ELECTIONRESULT_H

// Qt Includes

// Project Includes
#include "star/election.h"

// TODO: Add operator==()

namespace Star
{

class ElectionResult
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    const Election* mElection;
    QString mWinner;
    QSet<QString> mUnresolvedNominees;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ElectionResult();
    ElectionResult(const Election* election, const QString& winner, const QSet<QString> unresolved);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;
    bool isComplete() const;
    QString winner() const;
    QSet<QString> unresolvedNominees() const;
    const Election* election() const;
};

}

#endif // ELECTIONRESULT_H
