// Unit Include
#include "resultspresenter.h"

// Standard Library Includes
#include <iostream>

// Qx Includes
#include <qx/core/qx-iostream.h>
#include <qx/core/qx-table.h>
#include <qx/core/qx-string.h>
#include <qx/utility/qx-helpers.h>

// Using
using Qx::cout;
using Qt::endl;

//===============================================================================================================
// ResultPresenter
//===============================================================================================================

//-Constructor-------------------------------------------------------------
ResultPresenter::ResultPresenter(const QList<Star::ElectionResult>* results, bool summaryOnly) :
    mResults(results),
    mSummaryOnly(summaryOnly)
{}

//-Instance Functions-------------------------------------------------------------
//Private:
void ResultPresenter::pause()
{
    // Print prompt
    cout << USER_PAUSE_PROMPT << Qt::flush;

    // Wait for ENTER key
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    // Remove prompt text
    cout << '\r' << QString(USER_PAUSE_PROMPT.size(), ' ') << '\r';
}

void ResultPresenter::printElectionResult(const Star::ElectionResult& result)
{
    QString category = result.election()->name();
    QStringList nominees = result.election()->nominees();
    // Print category
    cout << HEADING_CATEGORY.arg(category) << endl << endl;

    // Print nominees
    cout << HEADING_NOMINEES << endl;
    for(const QString& nom : nominees)
        cout << '"' << nom << '"' << endl;
    cout << endl;

    // Wait on user confirm
    pause();

    // Print result
    const QStringList& winners = result.winners();
    cout << HEADING_WINNERS << endl;
    for(qsizetype w = 0; w < winners.size(); w++)
        cout << WINNER_TEMPLATE.arg(w).arg(winners.at(w)) << endl;

    // Print raw score rankings
    for(const Rank& rank : result.election()->scoreRankings())
        cout << RAW_SCORE_TEMPLATE.arg(Qx::String::join(rank.nominees, R"(", ")")).arg(rank.value) << endl;
    cout << endl;

    // Wait on user confirm
    pause();
}

void ResultPresenter::printMainHeading()
{
    cout << HEADING_RESULTS << "\n\n" << endl;
}

void ResultPresenter::printResults()
{
    for(const Star::ElectionResult& res : *mResults)
        printElectionResult(res);
}

void ResultPresenter::printSummary()
{
    //-Create results summary table---------------------------
    Qx::Table<QString> summaryTable(QSize(2, mResults->size() + 1)); // +1 for heading

    // Add headings
    summaryTable.at(0, 0) = SUMMARY_HEADING_CATEGORY;
    summaryTable.at(0, 1) = SUMMARY_HEADING_WINNER;

    // Add results
    for(int res = 0, row = 1; res < mResults->size(); res++, row++)
    {
        // Result
        const Star::ElectionResult& result = mResults->at(res);

        // Category, winner, runner-up
        summaryTable.at(row, 0) = ' ' + result.election()->name() + ' ';
        summaryTable.at(row, 1) = result.winners().front();
    }

    // Determine field widths
    std::array<qsizetype, 3> fieldWidths{0, 0, 0};
    for(auto itr = summaryTable.rowBegin(); itr != summaryTable.rowEnd(); itr++)
    {
        for(qsizetype c = 0; c < 3; c++)
            fieldWidths[c] = std::max(fieldWidths[c], itr->at(c).size());
    }

    //-Print summaries---------------------------------------
    cout << Qt::left;

    auto printTableRow = [&fieldWidths](const QList<QString>& row){
        for(qsizetype i = 0; i < 3; i++)
            cout << SUMMARY_VERTICAL_SEP << qSetFieldWidth(fieldWidths[i]) << row[i] << qSetFieldWidth(0);
        cout << SUMMARY_VERTICAL_SEP << endl;
    };

    // Print section heading
    cout << HEADING_SUMMARY << endl << endl;

    // Print table heading row
    printTableRow(summaryTable.takeFirstRow());

    // Print horizontal divider
    QString div;
    for(qsizetype i = 0; i < 3; i++)
        div += SUMMARY_VERTICAL_SEP + QString(fieldWidths[i], SUMMARY_HORIZONTAL_SEP);
    div += SUMMARY_VERTICAL_SEP;

    cout << div << endl;

    // Print category rows
    for(auto itr = summaryTable.rowBegin(); itr != summaryTable.rowEnd(); itr++)
        printTableRow(*itr);
    cout << endl;
}

//Public:
void ResultPresenter::present()
{
    if(!mSummaryOnly)
    {
        printMainHeading();
        printResults();
    }

    printSummary();
}
