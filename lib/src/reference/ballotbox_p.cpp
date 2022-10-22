// Unit Include
#include "ballotbox_p.h"

// Qx Includes
#include <qx/core/qx-dsvtable.h>
#include <qx/io/qx-common-io.h>

// Project Includes
#include "categoryconfig_p.h"

namespace Star
{

//===============================================================================================================
// RefBallotBox
//===============================================================================================================

//-Constructor--------------------------------------------------------------------------------------------------------
//Public:
RefBallotBox::RefBallotBox() {}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
const QList<RefCategory>& RefBallotBox::categories() const { return mCategories; }
const QList<RefBallot>& RefBallotBox::ballots() const { return mBallots; }

//===============================================================================================================
// RefBallotBox::Reader
//===============================================================================================================

//-Constructor-----------------------------------------------------------------------------------------------------
//Protected:
RefBallotBox::Reader::Reader(RefBallotBox* targetBox, const QString& filePath, const RefCategoryConfig* categoryConfig) :
    mTargetBox(targetBox),
    mCsvFile(filePath),
    mCategoryConfig(categoryConfig),
    mExpectedFieldCount(STATIC_FIELD_COUNT + mCategoryConfig->totalNominees())
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Private:
Qx::GenericError RefBallotBox::Reader::parseCategories(const QList<QVariant>& headingsRow)
{
    // Fill out categories
    qsizetype cIdx = STATIC_FIELD_COUNT; // Skip known headings

    for(const RefCategoryHeader& ch : mCategoryConfig->headers())
    {
        // Read nominees directly into Category
        RefCategory category{.name = ch.name, .nominees = {}};
        QStringList& nominees = category.nominees;

        for(uint i = 0; i < ch.nomineeCount; i++, cIdx++)
        {
            QString nomineeField = headingsRow[cIdx].toString().trimmed();
            if(nomineeField.isEmpty())
                return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_BLANK_VALUE.arg(0).arg(cIdx));

            if(nominees.contains(nomineeField))
                return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_DUPLICATE_NOMINEE);

            nominees += nomineeField;
        }

        // Add category to box
        mTargetBox->mCategories.append(category);
    }

    return Qx::GenericError();
}

Qx::GenericError RefBallotBox::Reader::parseBallot(const QList<QVariant>& ballotRow, qsizetype ballotNum)
{
    // Ignore lines with all empty fields
    bool allEmpty = true;
    for(const QVariant& field : ballotRow)
    {
        if(!field.toString().isEmpty())
        {
            allEmpty = false;
            break;
        }
    }

    if(allEmpty)
        return Qx::GenericError();

    // Read submission date
    QDate submitted = QDate::fromString(ballotRow[SUBMISSION_DATE_INDEX].toString(), "d-MMM-yy").addYears(100);
//    Since this value isn't actually used as part of the election, it doesn't really matter if this is valid
//    if(!submitted.isValid())
//        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_DATE);

    // Read voter name
    QString voterName = ballotRow[MEMBER_NAME_INDEX].toString();
    if(voterName.isEmpty())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_BLANK_VALUE.arg(ballotNum).arg(MEMBER_NAME_INDEX));

    // Create ballot with existing info
    RefBallot ballot{.voter = voterName, .submissionDate = submitted, .votes = {}};

    // Read votes by category
    qsizetype cIdx = STATIC_FIELD_COUNT; // Skip known headings

    for(const RefCategoryHeader& ch : mCategoryConfig->headers())
    {
        QList<uint> categoryVotes;

        for(uint i = 0; i < ch.nomineeCount; i++, cIdx++)
        {
            QString voteField = ballotRow[cIdx].toString().trimmed();

            // If field is blank, treat it as a 0
            if(voteField.isEmpty())
            {
                categoryVotes.append(0);
                continue;
            }

            // Get value from field
            bool validValue;
            uint vote = voteField.toUInt(&validValue);

            if(!validValue || vote > 5)
                return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_VOTE.arg(ballotNum).arg(cIdx));

            categoryVotes.append(vote);
        }

        ballot.votes.append(categoryVotes);
    }

    // Add ballot to box
    mTargetBox->mBallots.append(ballot);

    return Qx::GenericError();
}

//Public:
Qx::GenericError RefBallotBox::Reader::readInto()
{
    // Error tracking
    Qx::GenericError errorStatus;

    // Quickly check if file is empty
    if(Qx::fileIsEmpty(mCsvFile))
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_EMPTY);

    // Read whole CSV into memory as raw data
    QByteArray csv;
    Qx::IoOpReport readReport = Qx::readBytesFromFile(csv, mCsvFile);
    if(readReport.isFailure())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(readReport.outcomeInfo());

    // Parse into DsvTable
    Qx::DsvParseError dsvError;
    Qx::DsvTable csvTable = Qx::DsvTable::fromDsv(csv, ',', '"', &dsvError);
    if(dsvError.error() != Qx::DsvParseError::NoError)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(dsvError.errorString());

    // Ensure the minimum amount of rows are present
    if(csvTable.rowCount() < 3)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_ROW_COUNT);

    // Ensure the column count is correct
    if(csvTable.columnCount() != mExpectedFieldCount)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_COLUMN_COUNT.arg(csvTable.columnCount()).arg(mExpectedFieldCount));

    // Separate headings from ballot rows
    QList<QVariant> headingRow = csvTable.takeFirstRow();

    // Process headings
    if((errorStatus = parseCategories(headingRow)).isValid())
        return errorStatus;

    // Process ballots
    for(qsizetype i = 0; i < csvTable.rowCount(); i++)
    {
        if((errorStatus = parseBallot(csvTable.rowAt(i), i)).isValid())
            return errorStatus;
    }

    return errorStatus;
}

}
