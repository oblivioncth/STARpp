// Unit Include
#include "resultspresenter.h"

// Qx Includes
#include <qx/core/qx-iostream.h>
#include <iostream>

// Using
using Qx::cout;
using Qt::endl;

//===============================================================================================================
// ResultPresenter
//===============================================================================================================

//-Constructor-------------------------------------------------------------
ResultPresenter::ResultPresenter(const QList<Star::ElectionResult>* results) :
    mResults(results)
{}

//-Instance Functions-------------------------------------------------------------
//Private:
void ResultPresenter::pause()
{
    // Print prompt
    cout << USER_PAUSE_PROMPT;

    // Wait for ENTER key
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');

    // Remove prompt text
    cout << '\r' << QString(USER_PAUSE_PROMPT.size(), ' ') << '\r';
}

void ResultPresenter::printElectionResult(const Star::ElectionResult& result)
{
    QString category = result.election()->name();
    QStringList nominees = result.election()->nominees();
    QString winner = result.winner();
    QString runnerUp = result.runnerUp();
    const QString& outcomeStrTemplate = result.isTie() ? TIE_TEMPLATE : WINNER_TEMPLATE;
    QString outcomeStr = outcomeStrTemplate.arg(winner, runnerUp);

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
    cout << HEADING_OUTCOME << endl;
    cout << outcomeStr << endl << endl;

    // Print raw scores
    for(const QString& nom : nominees)
        cout << RAW_SCORE_TEMPLATE.arg(nom, result.election()->totalScore(nom)) << endl;
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
    // Determine field width
    qsizetype mcw = 0;
    for(const Star::ElectionResult& res : *mResults)
        mcw = std::max(mcw, res.election()->name().size());
    mcw += 1; // Account for ')'

    // Print heading
    cout << HEADING_SUMMARY << endl << endl;

    // Print result summaries
    for(const Star::ElectionResult& res : *mResults)
    {
        const QString& summaryEntryTemplate = res.isTie() ? SUMMARY_TIE_ITEM : SUMMARY_WINNER_ITEM;
        QString summaryEntry = summaryEntryTemplate.arg(res.winner(), res.runnerUp());

        cout << Qt::right << qSetFieldWidth(mcw);
        cout << res.election()->name() + ')';
        cout << Qt::left << qSetFieldWidth(0);
        cout << ' ' << summaryEntry << endl;
    }
    cout << endl;
}

//Public:
void ResultPresenter::present()
{
    printMainHeading();
    printResults();
    printSummary();
}
