// Unit Include
#include "categoryconfig_p.h"

// Qt Includes
#include <QFileInfo>

namespace Star
{

//===============================================================================================================
// RefCategoryConfig
//===============================================================================================================

//-Constructor--------------------------------------------------------------------------------------------------------
//Public:
RefCategoryConfig::RefCategoryConfig() :
    mHeaders(),
    mTotalNominees(0)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
uint RefCategoryConfig::totalNominees() const { return mTotalNominees; }
const QList<RefCategoryHeader>& RefCategoryConfig::headers() const { return mHeaders; }

//===============================================================================================================
// RefCategoryConfig::Reader
//===============================================================================================================

//-Constructor-----------------------------------------------------------------------------------------------------
//Protected:
RefCategoryConfig::Reader::Reader(RefCategoryConfig* targetConfig, const QString& filePath) :
    mTargetConfig(targetConfig),
    mIniReader(filePath)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
Qx::GenericError RefCategoryConfig::Reader::readInto()
{
    QFileInfo iniInfo(mIniReader.filePath());

    // Before checking the reader status, see if the file even exists
    if(!iniInfo.exists())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_DOES_NOT_EXIST);

    // Also, directly check for empty file
    if(iniInfo.size() == 0)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_EMPTY);

    // Open file
    Qx::IoOpReport openReport = mIniReader.openFile();
    if(openReport.isFailure())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(openReport.outcomeInfo());

    // Read first line and ensure the group is correct
    QString sectionGroup = mIniReader.readLine().trimmed();
    if(sectionGroup != SECTION_HEADING)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_LAYOUT);

    // Read each key/value
    while(!mIniReader.atEnd())
    {
        QString keyValueStr = mIniReader.readLine();

        // Skip blank lines
        if(keyValueStr.isEmpty())
            continue;

        // Split key/value
        QStringList keyValueList = keyValueStr.split('=');
        if(keyValueList.size() != 2)
            return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_INI);

        QString key = keyValueList.at(0).trimmed();
        QString valueStr = keyValueList.at(1).trimmed();

        // Convert value to unsigned integer, ensure string was valid
        bool validValue;
        uint categoryCount = valueStr.toUInt(&validValue);

        if(!validValue || categoryCount < 2)
            return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_VALUE);

        // Add header to target config
        RefCategoryHeader ch{.name = key, .nomineeCount = categoryCount};
        mTargetConfig->mHeaders.append(ch);

        // Increase total nominee count
        mTargetConfig->mTotalNominees += categoryCount;
    }

    // Check stream status
    if(mIniReader.hasError())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(mIniReader.status().outcomeInfo());

    // Fail if there are no categories
    if(mTargetConfig->mHeaders.isEmpty())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_EMPTY);

    return Qx::GenericError();
}

}
