#ifndef CALCULATOROPTIONS_P_H
#define CALCULATOROPTIONS_P_H

// Qt Includes
#include <QString>
#include <QList>

// Qx Includes
#include <qx/core/qx-genericerror.h>
#include <qx/io/qx-textstreamreader.h>

// Project Includes
#include "star/calculator.h"

namespace Star
{
/*! @cond */

class CalcOptionsReader
{
//-Class Variables--------------------------------------------------------------------------------------------------
private:
    static inline const QString MAIN_ERR_MSG = QStringLiteral("Error reading the options list.");

    static inline const QString ERR_DOES_NOT_EXIST = QStringLiteral("The specified file does not exist.");
    static inline const QString ERR_INVALID_OPTION = QStringLiteral("The file contains an invalid option (Line: %1).");

    static inline const Qx::GenericError ERROR_TEMPLATE = Qx::GenericError(Qx::GenericError::Critical, MAIN_ERR_MSG);

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    Star::Calculator::Options* mTargetOptions;
    Qx::TextStreamReader mOptionsReader;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    CalcOptionsReader(Star::Calculator::Options* targetOptions, const QString& filePath);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    Qx::GenericError readInto();
};
/*! @endcond */
}
#endif // CALCULATOROPTIONS_P_H
