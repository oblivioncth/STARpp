#ifndef RESULTSSET_P_H
#define RESULTSSET_P_H

// Qt Includes
#include <QFile>

// Qx Includes
#include <qx/core/qx-error.h>

// Project Includes
#include "star/expectedelectionresult.h"

namespace Star
{
/*! @cond */
class ResultSetReader
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QList<ExpectedElectionResult>* mTargetList;
    QFile mFile;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    ResultSetReader(QList<ExpectedElectionResult>* targetList, const QString& filePath);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    Qx::Error readInto();
};
/*! @endcond */
}

#endif // RESULTSSET_P_H
