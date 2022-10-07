#ifndef BALLOTBOX_P_H
#define BALLOTBOX_P_H

// Qt Includes
#include <QString>
#include <QList>
#include <QDate>
#include <QFile>

// Qx Includes
#include <qx/core/qx-genericerror.h>

namespace Star
{

// Project Forward Declarations
class RefCategoryConfig;

struct RefCategory
{
    QString name;
    QStringList nominees;
};

struct RefBallot
{
    QString voter;
    QDate submissionDate;
    QList<QList<uint>> votes;
};

class RefBallotBox
{
//-Inner Classes----------------------------------------------------------------------------------------------------
public:
    class Reader;

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QList<RefCategory> mCategories;
    QList<RefBallot> mBallots;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    RefBallotBox();

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    const QList<RefCategory>& categories() const;
    const QList<RefBallot>& ballots() const;
};

class RefBallotBox::Reader
{
//-Class Variables--------------------------------------------------------------------------------------------------
private:
    // Error
    static inline const QString MAIN_ERR_MSG = QStringLiteral("Error reading the ballot box");

    static inline const QString ERR_INVALID_ROW_COUNT = QStringLiteral("The ballot box has less than the minimum required row count (headings + 2 voters).");
    static inline const QString ERR_INVALID_COLUMN_COUNT = QStringLiteral("A row had a different number of columns than specified by the category configuration.");
    static inline const QString ERR_INVALID_DATE = QStringLiteral("The format of a submission date was invalid.");
    static inline const QString ERR_EMPTY = QStringLiteral("The provided file contains no ballots.");
    static inline const QString ERR_BLANK_VALUE = QStringLiteral("A field expected to have a value was blank.");
    static inline const QString ERR_INVALID_VOTE = QStringLiteral("A vote value was not a valid unsigned integer between 0 and 5.");

    static inline const Qx::GenericError ERROR_TEMPLATE = Qx::GenericError(Qx::GenericError::Critical, MAIN_ERR_MSG);

    // Fields
    static const int STATIC_FIELD_COUNT = 2;
    static const int SUBMISSION_DATE_INDEX = 0;
    static const int MEMBER_NAME_INDEX = 1;

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    RefBallotBox* mTargetBox;
    QFile mCsvFile;
    const RefCategoryConfig* mCategoryConfig;
    uint mExpectedFieldCount;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    Reader(RefBallotBox* targetBox, const QString& filePath, const RefCategoryConfig* categoryConfig);

//-Instance Functions-------------------------------------------------------------------------------------------------
private:
    Qx::GenericError parseCategories(const QString& headingsRow);
    Qx::GenericError parseBallot(const QString& ballotRow);

public:
    Qx::GenericError readInto();
};

}

#endif //BALLOTBOX_P_H
