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
    QString mWinner;
    QString mRunnerUp;
    bool mTie;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ElectionResult();
    ElectionResult(const Election* election, const QString& winner, const QString& runnerUp, bool tie);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;

    const Election* election() const;
    QString winner() const;
    QString runnerUp() const;
    bool isTie() const;
};

}

#endif // ELECTIONRESULT_H
