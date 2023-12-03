#ifndef CALCULATOROPTIONS_P_H
#define CALCULATOROPTIONS_P_H

// Qt Includes
#include <QString>
#include <QList>

// Qx Includes
#include <qx/io/qx-textstreamreader.h>

// Project Includes
#include "star/calculator.h"

namespace Star
{
/*! @cond */

class QX_ERROR_TYPE(CalcOptionsError, "Star::CalcOptionsError", 1151)
{
    friend class CalcOptionsReader;
//-Class Enums-------------------------------------------------------------
public:
    enum Type
    {
        NoError,
        DoesNotExist,
        IoError,
        InvalidOption
    };

//-Class Variables-------------------------------------------------------------
private:
    static inline const QString MAIN_ERR_MSG = u"Error reading the options list."_s;
    static inline const QHash<Type, QString> ERR_STRINGS{
        {NoError, u""_s},
        {NoError, u"IO Error: %1"_s},
        {DoesNotExist, u"The specified file does not exist."_s},
        {InvalidOption, u"The file contains an invalid option (Line: %1)."_s}
    };

//-Instance Variables-------------------------------------------------------------
private:
    Type mType;
    QString mString;

//-Constructor-------------------------------------------------------------
private:
    CalcOptionsError(Type t = NoError);

    template<typename... Strings>
        requires (std::same_as<Strings, QString> && ...)
    CalcOptionsError(Type t, Strings... args) :
        CalcOptionsError(t)
    {
        mString = mString.arg(args...);
    }

    template<typename... Ints>
        requires (std::integral<Ints> && ...)
    CalcOptionsError(Type t, Ints... args) :
        CalcOptionsError(t)
    {
        auto sub = [this](auto integer) { mString = mString.arg(integer); };
        (sub(args), ...);
    }

//-Instance Functions-------------------------------------------------------------
public:
    bool isValid() const;
    Type type() const;
    QString string() const;

private:
    Qx::Severity deriveSeverity() const override;
    quint32 deriveValue() const override;
    QString derivePrimary() const override;
    QString deriveSecondary() const override;
};

class CalcOptionsReader
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    Star::Calculator::Options* mTargetOptions;
    Qx::TextStreamReader mOptionsReader;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    CalcOptionsReader(Star::Calculator::Options* targetOptions, const QString& filePath);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    CalcOptionsError readInto();
};
/*! @endcond */
}
#endif // CALCULATOROPTIONS_P_H
