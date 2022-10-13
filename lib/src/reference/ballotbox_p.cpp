// Unit Include
#include "ballotbox_p.h"

// Qx Includes
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
Qx::GenericError RefBallotBox::Reader::parseCategories(const QString& headingsRow)
{
    // Split row into fields
    QStringList headingFields = headingsRow.split(',');

    // Ensure field count is as expected
    if(headingFields.size() != mExpectedFieldCount)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_COLUMN_COUNT);

    // Fill out categories
    qsizetype cIdx = STATIC_FIELD_COUNT; // Skip known headings

    for(const RefCategoryHeader& ch : mCategoryConfig->headers())
    {
        // Read nominees directly into Category
        RefCategory category{.name = ch.name, .nominees = {}};
        QStringList& nominees = category.nominees;

        for(uint i = 0; i < ch.nomineeCount; i++, cIdx++)
        {
            QString nomineeField = headingFields.at(cIdx);
            if(nomineeField.isEmpty())
                return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_BLANK_VALUE);

            nominees += nomineeField;
        }

        // Add category to box
        mTargetBox->mCategories.append(category);
    }

    return Qx::GenericError();
}

Qx::GenericError RefBallotBox::Reader::parseBallot(const QString& ballotRow)
{
    // Ignore empty lines
    if(ballotRow.isEmpty())
        return Qx::GenericError();

    // Split row into fields
    QStringList ballotFields = ballotRow.split(',');

    // Ignore lines with all empty fields
    bool allEmpty = true;
    for(const QString& field : ballotFields)
    {
        if(!field.isEmpty())
        {
            allEmpty = false;
            break;
        }
    }

    if(allEmpty)
        return Qx::GenericError();

    // Ensure field count is as expected
    if(ballotFields.size() != mExpectedFieldCount)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_COLUMN_COUNT);

    // Read submission date
    QDate submitted = QDate::fromString(ballotFields[SUBMISSION_DATE_INDEX], "d-MMM-yy").addYears(100);
    if(!submitted.isValid())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_DATE);

    // Read voter name
    QString voterName = ballotFields[MEMBER_NAME_INDEX];
    if(voterName.isEmpty())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_BLANK_VALUE);

    // Create ballot with existing info
    RefBallot ballot{.voter = voterName, .submissionDate = submitted, .votes = {}};

    // Read votes by category
    qsizetype cIdx = STATIC_FIELD_COUNT; // Skip known headings

    for(const RefCategoryHeader& ch : mCategoryConfig->headers())
    {
        QList<uint> categoryVotes;

        for(uint i = 0; i < ch.nomineeCount; i++, cIdx++)
        {
            QString voteField = ballotFields.at(cIdx);

            // If field is blank, treat it as a 0
            if(voteField.isEmpty())
            {
                categoryVotes[i] = 0;
                break;
            }

            // Get value from field
            bool validValue;
            uint vote = voteField.toUInt(&validValue);

            if(!validValue || vote > 5)
                return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_VOTE);

            categoryVotes[i] = vote;
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
    /* TODO: May want to add a check that makes sure they're aren't duplicate category names, or other redundancies that would break things.
     * Unlikely to occur though as this would be an issue with input data collection
     */

    // Error tracking
    Qx::GenericError errorStatus;

    // Quickly check if file is empty
    if(Qx::fileIsEmpty(mCsvFile))
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_EMPTY);

    // Read whole CSV into memory as line list
    QStringList csvRows;
    Qx::IoOpReport readReport = Qx::readTextFromFile(csvRows, mCsvFile, Qx::Index32::FIRST, Qx::Index32::LAST, Qx::IgnoreTrailingBreak);
    if(readReport.isFailure())
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(readReport.outcomeInfo());

    // Ensure the minimum amount of rows are present
    if(csvRows.size() < 3)
        return Qx::GenericError(ERROR_TEMPLATE).setSecondaryInfo(ERR_INVALID_ROW_COUNT);

    // Separate headings from ballot rows
    QString headingRow = csvRows.takeFirst();

    // Process headings
    if((errorStatus = parseCategories(headingRow)).isValid())
        return errorStatus;

    // Process ballots
    for(const QString& ballotRow : csvRows)
    {
        if((errorStatus = parseBallot(ballotRow)).isValid())
            return errorStatus;
    }

    return errorStatus;
}

}
