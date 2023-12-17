#ifndef CATEGORYCONFIG_P_H
#define CATEGORYCONFIG_P_H

// Qt Includes
#include <QString>
#include <QList>

// Qx Includes
#include <qx/io/qx-textstreamreader.h>

namespace Star
{
/*! @cond */

struct RefCategoryHeader
{
    QString name;
    uint candidateCount;
};

class QX_ERROR_TYPE(RefCategoryConfigError, "Star::RefCategoryConfigError", 1152)
{
    friend class RefCategoryConfig;
//-Class Enums-------------------------------------------------------------
public:
    enum Type
    {
        NoError,
        DoesNotExist,
        Empty,
        IoError,
        InvalidIni,
        InvalidLayout,
        InvalidValueType,
        InvalidCategoryCount,
        DuplicateCategory,
        InvalidGeneralKey,
        InvalidSeatCount,
        NoCategories,
        NoSeats
    };

//-Class Variables-------------------------------------------------------------
private:
    static inline const QString MAIN_ERR_MSG = u"Error reading the category configuration."_s;
    static inline const QHash<Type, QString> ERR_STRINGS{
        {NoError, u""_s},
        {DoesNotExist, u"The specified file does not exist."_s},
        {Empty, u"The provided file is empty."_s},
        {IoError, u"IO Error: %1"_s},
        {InvalidIni, u"The provided file is not a valid INI format file (Line: %1)."_s},
        {InvalidLayout, u"The provided file does not follow the expected layout (Line: %1)."_s},
        {InvalidValueType, u"Category configuration contains a value that is not an unsigned integer (Line: %1)."_s},
        {InvalidCategoryCount, u"A category specified a candidate count less than 2 (Line: %1)."_s},
        {DuplicateCategory, u"The provided file contains duplicate categories (Line: %1)."_s},
        {InvalidGeneralKey, u"Unrecognized General key (Line: %1)."_s},
        {InvalidSeatCount, u"The provided file specified a seat count less than 1 (Line: %1)."_s},
        {NoCategories, u"The provided file contains no categories."_s},
        {NoSeats, u"The provided file didn't specify a seat count."_s}
    };

//-Instance Variables-------------------------------------------------------------
private:
    Type mType;
    QString mString;

//-Constructor-------------------------------------------------------------
private:
    RefCategoryConfigError(Type t = NoError);

    template<typename... Strings>
        requires (std::same_as<Strings, QString> && ...)
    RefCategoryConfigError(Type t, Strings... args) :
        RefCategoryConfigError(t)
    {
        mString = mString.arg(args...);
    }

    template<typename... Ints>
        requires (std::integral<Ints> && ...)
    RefCategoryConfigError(Type t, Ints... args) :
        RefCategoryConfigError(t)
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

class RefCategoryConfig
{
//-Inner Classes----------------------------------------------------------------------------------------------------
public:
    class Reader;

//-Class Variables--------------------------------------------------------------------------------------------------
private:
    static inline const QString SECTION_HEADING_CATEGORIES = QStringLiteral("[Categories]");
    static inline const QString SECTION_HEADING_GENERAL = QStringLiteral("[General]");
    static inline const QString KEY_GENERAL_SEATS = QStringLiteral("Seats");

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QList<RefCategoryHeader> mHeaders;
    uint mSeats;
    uint mTotalCandidates;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    RefCategoryConfig();

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    uint totalCandidates() const;
    uint seats() const;
    const QList<RefCategoryHeader>& headers() const;
};

class RefCategoryConfig::Reader
{
//-Class Enums------------------------------------------------------------------------------------------------------
private:
    enum Section { None, Categories, General };

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    RefCategoryConfig* mTargetConfig;
    Qx::TextStreamReader mIniReader;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    Reader(RefCategoryConfig* targetConfig, const QString& filePath);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    RefCategoryConfigError readInto();
};
/*! @endcond */
}
#endif // CATEGORYCONFIG_P_H
