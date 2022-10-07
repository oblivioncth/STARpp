// Unit Include
#include "categoryconfig_p.h"

// Qt Includes
#include <QFileInfo>

//===============================================================================================================
// CategoryConfig
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
// CategoryConfig::Reader
//===============================================================================================================

//-Constructor-----------------------------------------------------------------------------------------------------
//Protected:
RefCategoryConfig::Reader::Reader(RefCategoryConfig* targetConfig, const QString& filePath) :
    mTargetConfig(targetConfig),
    mIniReader(filePath, QSettings::IniFormat)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
Qx::GenericError RefCategoryConfig::Reader::readInto()
{
    QFileInfo iniInfo(mIniReader.fileName());

    // Before checking the reader status, see if the file even exists
    if(!iniInfo.exists())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_DOES_NOT_EXIST);

    // Also, directly check for empty file
    if(iniInfo.size() == 0)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_EMPTY);

    // Check that file was read OK
    if(mIniReader.status() == QSettings::Status::AccessError)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_ACCESS);
    if(mIniReader.status() == QSettings::Status::FormatError)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_INI);
    if(mIniReader.status() != QSettings::Status::NoError)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_UNKNOWN);

    // Check that the key layout is as expected
    if(mIniReader.childGroups() != QStringList{RefCategoryConfig::SECTION_HEADING})
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_LAYOUT);

    // Step down into the main key category
    mIniReader.beginGroup(RefCategoryConfig::SECTION_HEADING);

    // Get category list
    const QStringList categories = mIniReader.childKeys();

    // Fail if there are no categories
    if(categories.isEmpty())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_EMPTY);

    // Enumerate all categories
    for(const QString& category : categories)
    {
        // Get category count
        QString countStr = mIniReader.value(category).toString();

        // Convert to unsigned integer, ensure string was valid
        bool validValue;
        uint count = countStr.toUInt(&validValue);

        if(!validValue || count < 2)
            return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_VALUE);

        // Add header to target config
        RefCategoryHeader ch{.name = category, .nomineeCount = count};
        mTargetConfig->mHeaders.append(ch);

        // Increase total nominee count
        mTargetConfig->mTotalNominees += count;
    }

    // Return to root level (currently for posterity since there is only one group heading)
    mIniReader.endGroup();

    return Qx::GenericError();
}
