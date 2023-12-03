#ifndef BALLOTBOX_P_H
#define BALLOTBOX_P_H

// Qt Includes
#include <QString>
#include <QList>
#include <QDate>
#include <QFile>

// Qx Includes
#include <qx/core/qx-abstracterror.h>

namespace Star
{
/*! @cond */

// Project Forward Declarations
class RefCategoryConfig;

struct RefCategory
{
    QString name;
    QStringList candidates;
};

struct RefBallot
{
    QString voter;
    QDate submissionDate;
    QList<QList<int>> votes;
};

class QX_ERROR_TYPE(RefBallotBoxError, "Star::RefBallotBoxError", 1150)
{
    friend class RefBallotBox;
//-Class Enums-------------------------------------------------------------
public:
    enum Type
    {
        NoError,
        InvalidRowCount,
        InvalidColumnCount,
        InvalidDate,
        Empty,
        BlankValue,
        InvalidVote,
        DuplicateCandidate,
        IoError
    };

//-Class Variables-------------------------------------------------------------
private:
    static inline const QString MAIN_ERR_MSG = u"Error reading the ballot box."_s;
    static inline const QHash<Type, QString> ERR_STRINGS{
        {NoError, u""_s},
        {InvalidRowCount, u"The ballot box has less than the minimum required row count (headings + 2 voters)."_s},
        {InvalidColumnCount, u"The ballot box has a different number of columns than specified by the category configuration (%1 vs %2)."_s},
        {InvalidDate, u"The format of a submission date was invalid."_s},
        {Empty, u"The provided file contains no ballots."_s},
        {BlankValue, u"A field expected to have a value was blank (r: %1, c: %2)."_s},
        {InvalidVote, u"A vote value was not a valid unsigned integer between 0 and 5 (r: %1, c: %2)."_s},
        {DuplicateCandidate, u"The ballot box contained duplicate candidates within the same category."_s},
        {IoError, u"IO Error: %1"_s}
    };

//-Instance Variables-------------------------------------------------------------
private:
    Type mType;
    QString mString;

//-Constructor-------------------------------------------------------------
private:
    RefBallotBoxError(Type t = NoError);

    template<typename... Strings>
        requires (std::same_as<Strings, QString> && ...)
    RefBallotBoxError(Type t, Strings... args) :
        RefBallotBoxError(t)
    {
        mString = mString.arg(args...);
    }

    template<typename... Ints>
        requires (std::integral<Ints> && ...)
    RefBallotBoxError(Type t, Ints... args) :
        RefBallotBoxError(t)
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
    // Fields
    static const int STATIC_FIELD_COUNT = 2;
    static const int SUBMISSION_DATE_INDEX = 0;
    static const int MEMBER_NAME_INDEX = 1;

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    RefBallotBox* mTargetBox;
    QFile mCsvFile;
    const RefCategoryConfig* mCategoryConfig;
    qsizetype mExpectedFieldCount;

//-Constructor--------------------------------------------------------------------------------------------------------
public:
    Reader(RefBallotBox* targetBox, const QString& filePath, const RefCategoryConfig* categoryConfig);

//-Instance Functions-------------------------------------------------------------------------------------------------
private:
    RefBallotBoxError parseCategories(const QList<QVariant>& headingsRow);
    RefBallotBoxError parseBallot(const QList<QVariant>& ballotRow, qsizetype ballotNum);

public:
    RefBallotBoxError readInto();
};
/*! @endcond */
}

#endif //BALLOTBOX_P_H
