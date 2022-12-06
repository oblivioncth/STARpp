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
    QSet<QString> mUnresolvedNominees;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ExpectedElectionResult();
    ExpectedElectionResult(const QStringList& winners, const QSet<QString> unresolved);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;

    QStringList winners() const;
    QSet<QString> unresolvedNominees() const;

    void setWinners(const QStringList& winners);
    void setUnresolvedNominees(QSet<QString> unresolved);

    bool operator==(const ElectionResult& result) const;
    bool operator!=(const ElectionResult& result) const;
};

}

#endif // EXPECTEDELECTIONRESULT_H
