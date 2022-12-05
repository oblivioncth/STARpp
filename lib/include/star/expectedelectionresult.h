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
    QSet<QString> mUnresolvedNominees;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ExpectedElectionResult();
    ExpectedElectionResult(const QString& winner, const QSet<QString> unresolved);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;

    QString winner() const;
    QSet<QString> unresolvedNominees() const;

    void setWinner(const QString& winner);
    void setUnresolvedNominees(QSet<QString> unresolved);

    bool operator==(const ElectionResult& result) const;
};

}

#endif // EXPECTEDELECTIONRESULT_H
