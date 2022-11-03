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
    QSet<QString> mWinners;
    QSet<QString> mRunnerUps;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ExpectedElectionResult();
    ExpectedElectionResult(const QSet<QString>& winners, const QSet<QString>& runnerUps);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;

    const QSet<QString>& winners() const;
    const QSet<QString>& runnerUps() const;

    void setWinners(const QSet<QString>& winners);
    void setRunnerUps(const QSet<QString>& runnerUps);

    bool operator==(const ElectionResult& result) const;
};

}

#endif // EXPECTEDELECTIONRESULT_H
