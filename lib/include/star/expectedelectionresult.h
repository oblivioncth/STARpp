#ifndef EXPECTEDELECTIONRESULT_H
#define EXPECTEDELECTIONRESULT_H

// Qt Includes
#include <QStringList>

// Project Includes
#include "star/electionresult.h"

namespace Star
{

class ExpectedElectionResult
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QStringList mWinners;
    QStringList mRunnerUps;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ExpectedElectionResult();
    ExpectedElectionResult(const QStringList& winners, const QStringList& runnerUps);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;

    const QStringList& winners() const;
    const QStringList& runnerUps() const;

    void setWinners(const QStringList& winners);
    void setRunnerUps(const QStringList& runnerUps);

    bool operator==(const ElectionResult& result) const;
};

}

#endif // EXPECTEDELECTIONRESULT_H
