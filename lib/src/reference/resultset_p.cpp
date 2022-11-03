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
        // Get object
        if(!value.isObject())
            return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_WRONG_ARRAY_ITEM_TYPE);
        QJsonObject expectedResultObj = value.toObject();

        // Get expected keys
        Qx::GenericError convError;

        QJsonArray jWinnerArray;
        QJsonArray jRunnerUpArray;
        if((convError = Qx::Json::checkedKeyRetrieval(jWinnerArray, expectedResultObj, KEY_WINNERS_ARRAY)).isValid())
            return convError;
        if((convError = Qx::Json::checkedKeyRetrieval(jRunnerUpArray, expectedResultObj, KEY_RUNNERUPS_ARRAY)).isValid())
            return convError;

        // Convert arrays to string lists
        QList<QString> winners;
        QList<QString> runnerUps;
        if((convError = Qx::Json::checkedArrayConversion(winners, jWinnerArray)).isValid())
            return convError;
        if((convError = Qx::Json::checkedArrayConversion(runnerUps, jRunnerUpArray)).isValid())
            return convError;

        // Convert to sets
        QSet<QString> winnerSet = QSet<QString>(winners.constBegin(), winners.constEnd());
        QSet<QString> runnerUpSet = QSet<QString>(runnerUps.constBegin(), runnerUps.constEnd());

        // Add expected result to list
        mTargetList->append(ExpectedElectionResult(winnerSet, runnerUpSet));
    }

    return Qx::GenericError();
}

}
