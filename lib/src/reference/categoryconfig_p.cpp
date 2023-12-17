// Unit Include
#include "categoryconfig_p.h"

// Qt Includes
#include <QFileInfo>

namespace Star
{
/*! @cond */
//===============================================================================================================
// RefCategoryConfigError
//===============================================================================================================

//-Constructor--------------------------------------------------------------------
RefCategoryConfigError::RefCategoryConfigError(Type t) :
    mType(t),
    mString(ERR_STRINGS.value(t))
{}

//-Instance Functions-------------------------------------------------------------
//Public:
bool RefCategoryConfigError::isValid() const { return mType != NoError; }
RefCategoryConfigError::Type RefCategoryConfigError::type() const { return mType; }
QString RefCategoryConfigError::string() const { return mString; }

//Private:
Qx::Severity RefCategoryConfigError::deriveSeverity() const { return Qx::Critical; }
quint32 RefCategoryConfigError::deriveValue() const { return mType; }
QString RefCategoryConfigError::derivePrimary() const { return MAIN_ERR_MSG; }
QString RefCategoryConfigError::deriveSecondary() const { return mString; }

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
RefCategoryConfigError RefCategoryConfig::Reader::readInto()
{
    QFileInfo iniInfo(mIniReader.filePath());

    // Before checking the reader status, see if the file even exists
    if(!iniInfo.exists())
        return RefCategoryConfigError(RefCategoryConfigError::DoesNotExist);

    // Also, directly check for empty file
    if(iniInfo.size() == 0)
        return RefCategoryConfigError(RefCategoryConfigError::Empty);

    // Open file
    Qx::IoOpReport openReport = mIniReader.openFile();
    if(openReport.isFailure())
        return RefCategoryConfigError(RefCategoryConfigError::IoError, openReport.outcomeInfo());

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
                return RefCategoryConfigError(RefCategoryConfigError::InvalidLayout, line);
        }
        else if(currentSection == Section::None) // No Section
            return RefCategoryConfigError(RefCategoryConfigError::InvalidIni, line);
        else // Key/Value
        {
            // Split key/value
            QStringList keyValueList = iniLine.split('=');
            if(keyValueList.size() != 2)
                return RefCategoryConfigError(RefCategoryConfigError::InvalidIni, line);

            QString key = keyValueList.at(0).trimmed();
            QString valueStr = keyValueList.at(1).trimmed();

            // Convert value to unsigned integer, ensure string was valid
            bool validValue;
            uint value = valueStr.toUInt(&validValue);

            if(!validValue)
                return RefCategoryConfigError(RefCategoryConfigError::InvalidValueType, line);

            // Sections
            if(currentSection == Section::Categories)
            {
                // Ensure value is valid
                if(value < 2)
                    return RefCategoryConfigError(RefCategoryConfigError::InvalidCategoryCount, line);

                // Make sure this isn't a duplicate
                auto start = mTargetConfig->headers().constBegin();
                auto end = mTargetConfig->headers().constEnd();
                bool dupe = (std::find_if(start, end, [&key](const RefCategoryHeader& header){
                    return header.name == key;
                }) != end);

                if(dupe)
                    return RefCategoryConfigError(RefCategoryConfigError::DuplicateCategory, line);

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
                        return RefCategoryConfigError(RefCategoryConfigError::InvalidSeatCount, line);

                    mTargetConfig->mSeats = value;
                }
                else
                    return RefCategoryConfigError(RefCategoryConfigError::InvalidGeneralKey, line);
            }
            else
                qCritical("Unhandled section");
        }
    }

    // Check stream status
    if(mIniReader.hasError())
        return RefCategoryConfigError(RefCategoryConfigError::IoError, mIniReader.status().outcomeInfo());

    // Fail if there are no categories
    if(mTargetConfig->mHeaders.isEmpty())
        return RefCategoryConfigError(RefCategoryConfigError::NoCategories);

    // Fail if seat count wasn't specified
    if(mTargetConfig->mSeats < 1)
        return RefCategoryConfigError(RefCategoryConfigError::NoSeats);

    return RefCategoryConfigError();
}
/*! @endcond */
}
