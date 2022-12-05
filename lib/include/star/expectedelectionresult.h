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
    QString mWinner;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ExpectedElectionResult();
    ExpectedElectionResult(const QString& winner);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;

    QString winner() const;

    void setWinner(const QString& winner);

    bool operator==(const ElectionResult& result) const;
};

}

#endif // EXPECTEDELECTIONRESULT_H
