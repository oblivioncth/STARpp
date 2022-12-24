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
/*! @cond */
class ResultSetReader
{
//-Class Variables--------------------------------------------------------------------------------------------------
private:
    // Keys
    static inline const QString KEY_WINNER_STR = QStringLiteral("winner");
    static inline const QString KEY_QUALIFIER_OBJ = QStringLiteral("qualifier");
    static inline const QString KEY_QUALIFIER_FIRST_ADV_ARRAY = QStringLiteral("firstAdv");
    static inline const QString KEY_QUALIFIER_SECOND_ADV_ARRAY = QStringLiteral("secondAdv");

    // Errors
    static inline const QString MAIN_ERR_MSG = QStringLiteral("Error reading results set.");

    static inline const QString ERR_WRONG_ROOT_TYPE = QStringLiteral("Root item is not an array.");
    static inline const QString ERR_WRONG_ROOT_ARRAY_ITEM_TYPE = QStringLiteral("Root array did not contain other arrays.");
    static inline const QString ERR_WRONG_SEAT_ARRAY_ITEM_TYPE = QStringLiteral("Seat array did not contain objects.");

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
/*! @endcond */
}

#endif // RESULTSSET_P_H
