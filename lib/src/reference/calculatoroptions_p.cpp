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
Qx::GenericError CalcOptionsReader::readInto()
{
    QFileInfo optInfo(mOptionsReader.filePath());

    // Before checking the reader status, see if the file even exists
    if(!optInfo.exists())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_DOES_NOT_EXIST);

    // Also, directly check for empty file
    if(optInfo.size() == 0)
        return Qx::GenericError();

    // Open file
    Qx::IoOpReport openReport = mOptionsReader.openFile();
    if(openReport.isFailure())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(openReport.outcomeInfo());


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
            return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_OPTION.arg(line));
    }

    // Check stream status
    if(mOptionsReader.hasError())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(mOptionsReader.status().outcomeInfo());

    return Qx::GenericError();
}
/*! @endcond */
}
