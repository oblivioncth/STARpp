#ifndef CATEGORYCONFIG_P_H
#define CATEGORYCONFIG_P_H

// Qt Includes
#include <QString>
#include <QList>
#include <QSettings>

// Qx Includes
#include <qx/core/qx-genericerror.h>

namespace Star
{

struct RefCategoryHeader
{
    QString name;
    uint nomineeCount;
};

class RefCategoryConfig
{
//-Inner Classes----------------------------------------------------------------------------------------------------
public:
    class Reader;

//-Class Variables--------------------------------------------------------------------------------------------------
private:
    static inline const QString SECTION_HEADING = QStringLiteral("Categories");

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QList<RefCategoryHeader> mHeaders;
    uint mTotalNominees;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    RefCategoryConfig();

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    uint totalNominees() const;
    const QList<RefCategoryHeader>& headers() const;
};

class RefCategoryConfig::Reader
{
//-Class Variables--------------------------------------------------------------------------------------------------
private:
    static inline const QString MAIN_ERR_MSG = QStringLiteral("Error reading the category configuration");

    static inline const QString ERR_DOES_NOT_EXIST = QStringLiteral("The specified file does not exist.");
    static inline const QString ERR_ACCESS = QStringLiteral("Failed to access the file data.");
    static inline const QString ERR_UNKNOWN = QStringLiteral("An unknown error occurred.");
    static inline const QString ERR_INVALID_INI = QStringLiteral("The provided file is not a valid INI format file.");
    static inline const QString ERR_INVALID_LAYOUT = QStringLiteral("The provided file does not follow the expected layout.");
    static inline const QString ERR_INVALID_VALUE = QStringLiteral("Category configuration contains a value that is not an unsigned integer greater than one.");
    static inline const QString ERR_EMPTY = QStringLiteral("The provided file contains no categories.");

    static inline const Qx::GenericError ERROR_TEMPLATE = Qx::GenericError(Qx::GenericError::Critical, MAIN_ERR_MSG);

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    RefCategoryConfig* mTargetConfig;
    QSettings mIniReader;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    Reader(RefCategoryConfig* targetConfig, const QString& filePath);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    Qx::GenericError readInto();
};

}
#endif // CATEGORYCONFIG_P_H
