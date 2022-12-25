// Unit Include
#include "resultset_p.h"

// Qt Includes
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

// Qx Includes
#include <qx/io/qx-common-io.h>
#include <qx/core/qx-json.h>

namespace Star
{
/*! @cond */
//===============================================================================================================
// ResultSetReader
//===============================================================================================================

//-Constructor-----------------------------------------------------------------------------------------------------
//Protected:
ResultSetReader::ResultSetReader(QList<ExpectedElectionResult>* targetList, const QString& filePath) :
    mTargetList(targetList),
    mFile(filePath)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
Qx::GenericError ResultSetReader::readInto()
{
    // Clear return buffer
    mTargetList->clear();

    // Read file
    QByteArray jData;
    Qx::IoOpReport readReport = Qx::readBytesFromFile(jData, mFile);
    if(readReport.isFailure())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(readReport.outcomeInfo());

    // Parse JSON
    QJsonParseError jError;
    QJsonDocument jDoc = QJsonDocument::fromJson(jData, &jError);
    if(jError.error != QJsonParseError::NoError)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(jError.errorString());

    // Get root value, should be array
    if(!jDoc.isArray())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_WRONG_ROOT_TYPE);
    QJsonArray jArray = jDoc.array();

    // Iterate array to fill target list
    for(const QJsonValue& value : jArray)
    {
        // Get array
        if(!value.isArray())
            return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_WRONG_ROOT_ARRAY_ITEM_TYPE);
        QJsonArray expectedResultSeatArray = value.toArray();

        // Iterate seat array to create expected result
        Star::ExpectedElectionResult::Builder eerb;
        for(const QJsonValue& jValueSeat : expectedResultSeatArray)
        {
            // Get object
            if(!jValueSeat.isObject())
                return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_WRONG_SEAT_ARRAY_ITEM_TYPE);
            QJsonObject seatObj = jValueSeat.toObject();

            // Get expected keys
            Qx::GenericError convError;

            QString winner;
            QJsonObject jQualifierObj;
            if((convError = Qx::Json::checkedKeyRetrieval(winner, seatObj, KEY_WINNER_STR)).isValid())
                return convError;
            if((convError = Qx::Json::checkedKeyRetrieval(jQualifierObj, seatObj, KEY_QUALIFIER_OBJ)).isValid())
                return convError;

            // Get expected keys for qualifier
            QJsonArray jFirstAdvArray;
            QJsonArray jSecondAdvArray;
            if((convError = Qx::Json::checkedKeyRetrieval(jFirstAdvArray, jQualifierObj, KEY_QUALIFIER_FIRST_ADV_ARRAY)).isValid())
                return convError;
            if((convError = Qx::Json::checkedKeyRetrieval(jSecondAdvArray, jQualifierObj, KEY_QUALIFIER_SECOND_ADV_ARRAY)).isValid())
                return convError;

            // Convert qualifier arrays to string lists
            QList<QString> firstAdv;
            QList<QString> secondAdv;
            if((convError = Qx::Json::checkedArrayConversion(firstAdv, jFirstAdvArray)).isValid())
                return convError;
            if((convError = Qx::Json::checkedArrayConversion(secondAdv, jSecondAdvArray)).isValid())
                return convError;

            // Convert to set
            QSet<QString> firstAdvSet = QSet<QString>(firstAdv.constBegin(), firstAdv.constEnd());
            QSet<QString> secondAdvSet = QSet<QString>(secondAdv.constBegin(), secondAdv.constEnd());

            // Add seat to builder
            eerb.wSeat(Seat(winner, QualifierResult(firstAdvSet, secondAdvSet)));
        }

        // Build expected result and add to target list
        mTargetList->append(eerb.build());
    }

    return Qx::GenericError();
}
/*! @endcond */
}
