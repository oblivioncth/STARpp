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
    QSet<QString> mWinners;
    QSet<QString> mRunnerUps;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ElectionResult();
    ElectionResult(const Election* election, const QSet<QString>& winners, const QSet<QString>& runnerUps);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;

    const Election* election() const;
    const QSet<QString>& winners() const;
    const QSet<QString>& runnerUps() const;
};

}

#endif // ELECTIONRESULT_H
