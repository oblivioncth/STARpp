// Unit Include
#include "categoryconfig_p.h"

// Qt Includes
#include <QFileInfo>

namespace Star
{
/*! @cond */
//===============================================================================================================
// RefCategoryConfig
//===============================================================================================================

//-Constructor--------------------------------------------------------------------------------------------------------
//Public:
RefCategoryConfig::RefCategoryConfig() :
    mHeaders(),
    mSeats(0),
    mTotalCandidates(0)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
uint RefCategoryConfig::totalCandidates() const { return mTotalCandidates; }
uint RefCategoryConfig::seats() const { return mSeats; }
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

    // Set default section
    Section currentSection = Section::None;

    // Read file line-by-line
    uint line = -1;
    while(!mIniReader.atEnd())
    {
        QString iniLine = mIniReader.readLine().trimmed();
        line++;

        // Skip blank lines
        if(iniLine.isEmpty())
            continue;

        // Handle each phase of processing
        if(iniLine.front() == '[') // Section Change
        {
            if(iniLine == RefCategoryConfig::SECTION_HEADING_CATEGORIES)
                currentSection = Section::Categories;
            else if(iniLine == RefCategoryConfig::SECTION_HEADING_GENERAL)
                currentSection = Section::General;
            else
                return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_LAYOUT.arg(line));
        }
        else if(currentSection == Section::None) // No Section
            return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_INI.arg(line));
        else // Key/Value
        {
            // Split key/value
            QStringList keyValueList = iniLine.split('=');
            if(keyValueList.size() != 2)
                return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_INI.arg(line));

            QString key = keyValueList.at(0).trimmed();
            QString valueStr = keyValueList.at(1).trimmed();

            // Convert value to unsigned integer, ensure string was valid
            bool validValue;
            uint value = valueStr.toUInt(&validValue);

            if(!validValue)
                return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_VALUE_TYPE.arg(line));

            // Sections
            if(currentSection == Section::Categories)
            {
                // Ensure value is valid
                if(value < 2)
                    return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_CATEGORY_COUNT.arg(line));

                // Make sure this isn't a duplicate
                auto start = mTargetConfig->headers().constBegin();
                auto end = mTargetConfig->headers().constEnd();
                bool dupe = (std::find_if(start, end, [&key](const RefCategoryHeader& header){
                    return header.name == key;
                }) != end);

                if(dupe)
                    return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_DUPLICATE_CATEGORY.arg(line));

                // Add header to target config
                RefCategoryHeader ch{.name = key, .candidateCount = value};
                mTargetConfig->mHeaders.append(ch);

                // Increase total candidate count
                mTargetConfig->mTotalCandidates += value;
            }
            else if(currentSection == Section::General)
            {
                // Keys
                if(key == RefCategoryConfig::KEY_GENERAL_SEATS)
                {
                    // Ensure value is valid
                    if(value < 1)
                        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_SEAT_COUNT.arg(line));

                    mTargetConfig->mSeats = value;
                }
                else
                    return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_GENERAL_KEY.arg(line));
            }
            else
                qCritical("Unhandled section");
        }
    }

    // Check stream status
    if(mIniReader.hasError())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(mIniReader.status().outcomeInfo());

    // Fail if there are no categories
    if(mTargetConfig->mHeaders.isEmpty())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_NO_CATEGORIES);

    // Fail if seat count wasn't specified
    if(mTargetConfig->mSeats < 1)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_NO_SEATS);

    return Qx::GenericError();
}
/*! @endcond */
}
