// Unit Include
#include "calculatoroptions_p.h"

// Qt Includes
#include <QFileInfo>

// Magic Enum Includes
#include <magic_enum.hpp>

namespace Star
{
/*! @cond */
//===============================================================================================================
// CalcOptionsError
//===============================================================================================================

//-Constructor--------------------------------------------------------------------
CalcOptionsError::CalcOptionsError(Type t) :
    mType(t),
    mString(ERR_STRINGS.value(t))
{}

//-Instance Functions-------------------------------------------------------------
//Public:
bool CalcOptionsError::isValid() const { return mType != NoError; }
CalcOptionsError::Type CalcOptionsError::type() const { return mType; }
QString CalcOptionsError::string() const { return mString; }

//Private:
Qx::Severity CalcOptionsError::deriveSeverity() const { return Qx::Critical; }
quint32 CalcOptionsError::deriveValue() const { return mType; }
QString CalcOptionsError::derivePrimary() const { return MAIN_ERR_MSG; }
QString CalcOptionsError::deriveSecondary() const { return mString; }


//===============================================================================================================
// RefCalcOptionsReader
//===============================================================================================================

//-Constructor-----------------------------------------------------------------------------------------------------
//Protected:
CalcOptionsReader::CalcOptionsReader(Star::Calculator::Options* targetOptions, const QString& filePath) :
    mTargetOptions(targetOptions),
    mOptionsReader(filePath)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
CalcOptionsError CalcOptionsReader::readInto()
{
    QFileInfo optInfo(mOptionsReader.filePath());

    // Before checking the reader status, see if the file even exists
    if(!optInfo.exists())
        return CalcOptionsError(CalcOptionsError::DoesNotExist);

    // Also, directly check for empty file
    if(optInfo.size() == 0)
        return CalcOptionsError();

    // Open file
    Qx::IoOpReport openReport = mOptionsReader.openFile();
    if(openReport.isFailure())
        return CalcOptionsError(CalcOptionsError::IoError, openReport.outcomeInfo());

    // Read file line-by-line
    for(uint line = 0; !mOptionsReader.atEnd(); line++)
    {
        QString optLine = mOptionsReader.readLine().trimmed();

        // Skip blank lines
        if(optLine.isEmpty())
            continue;

        // Try to convert line to calc option
        auto potentialOpt = magic_enum::enum_cast<Star::Calculator::Option>(optLine.toStdString());
        if(potentialOpt.has_value())
            mTargetOptions->setFlag(potentialOpt.value());
        else
            return CalcOptionsError(CalcOptionsError::InvalidOption, line);
    }

    // Check stream status
    if(mOptionsReader.hasError())
        CalcOptionsError(CalcOptionsError::IoError, mOptionsReader.status().outcomeInfo());

    return CalcOptionsError();
}
/*! @endcond */
}
