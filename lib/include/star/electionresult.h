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

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ElectionResult();
    ElectionResult(const Election* election, const QString& winner);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;

    const Election* election() const;
    QString winner() const;
};

}

#endif // ELECTIONRESULT_H
