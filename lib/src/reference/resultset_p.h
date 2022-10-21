#ifndef RESULTSSET_P_H
#define RESULTSSET_P_H

// Qt Includes
#include <QFile>

// Qx Includes
#include <qx/core/qx-genericerror.h>

// Project Includes
#include "star/expectedelectionresult.h"

namespace Star
{

class ResultSetReader
{
//-Class Variables--------------------------------------------------------------------------------------------------
private:
    // Keys
    static inline const QString KEY_WINNERS_ARRAY = QStringLiteral("winners");
    static inline const QString KEY_RUNNERUPS_ARRAY = QStringLiteral("runnerups");

    // Errors
    static inline const QString MAIN_ERR_MSG = QStringLiteral("Error reading results set.");

    static inline const QString ERR_WRONG_ROOT_TYPE = QStringLiteral("Root item is not an array.");
    static inline const QString ERR_WRONG_ARRAY_ITEM_TYPE = QStringLiteral("Root array did not contain objects.");

    static inline const Qx::GenericError ERROR_TEMPLATE = Qx::GenericError(Qx::GenericError::Critical, MAIN_ERR_MSG);

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QList<ExpectedElectionResult>* mTargetList;
    QFile mFile;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    ResultSetReader(QList<ExpectedElectionResult>* targetList, const QString& filePath);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    Qx::GenericError readInto();
};

}

#endif // RESULTSSET_P_H
