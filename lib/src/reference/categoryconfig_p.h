#ifndef CATEGORYCONFIG_P_H
#define CATEGORYCONFIG_P_H

// Qt Includes
#include <QString>
#include <QList>

// Qx Includes
#include <qx/core/qx-genericerror.h>
#include <qx/io/qx-textstreamreader.h>

namespace Star
{
/*! @cond */

struct RefCategoryHeader
{
    QString name;
    uint candidateCount;
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

//-Class Variables--------------------------------------------------------------------------------------------------
private:
    static inline const QString MAIN_ERR_MSG = QStringLiteral("Error reading the category configuration.");

    static inline const QString ERR_DOES_NOT_EXIST = QStringLiteral("The specified file does not exist.");
    static inline const QString ERR_EMPTY = QStringLiteral("The provided file is empty.");
    static inline const QString ERR_INVALID_INI = QStringLiteral("The provided file is not a valid INI format file (Line: %1).");
    static inline const QString ERR_INVALID_LAYOUT = QStringLiteral("The provided file does not follow the expected layout (Line: %1).");
    static inline const QString ERR_INVALID_VALUE_TYPE = QStringLiteral("Category configuration contains a value that is not an unsigned integer (Line: %1).");
    static inline const QString ERR_INVALID_CATEGORY_COUNT = QStringLiteral("A category specified a candidate count less than 2 (Line: %1).");
    static inline const QString ERR_DUPLICATE_CATEGORY = QStringLiteral("The provided file contains duplicate categories (Line: %1).");
    static inline const QString ERR_INVALID_GENERAL_KEY = QStringLiteral("Unrecognized General key (Line: %1).");
    static inline const QString ERR_INVALID_SEAT_COUNT = QStringLiteral("The provided file specified a seat count less than 1 (Line: %1).");
    static inline const QString ERR_NO_CATEGORIES = QStringLiteral("The provided file contains no categories.");
    static inline const QString ERR_NO_SEATS = QStringLiteral("The provided file didn't specify a seat count.");

    static inline const Qx::GenericError ERROR_TEMPLATE = Qx::GenericError(Qx::GenericError::Critical, MAIN_ERR_MSG);

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    RefCategoryConfig* mTargetConfig;
    Qx::TextStreamReader mIniReader;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    Reader(RefCategoryConfig* targetConfig, const QString& filePath);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    Qx::GenericError readInto();
};
/*! @endcond */
}
#endif // CATEGORYCONFIG_P_H
