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
    QStringList mRunnerUps;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ElectionResult();
    ElectionResult(const Election* election, const QStringList& winners, const QStringList& runnerUps);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;

    const Election* election() const;
    const QStringList& winners() const;
    const QStringList& runnerUps() const;
};

}

#endif // ELECTIONRESULT_H
