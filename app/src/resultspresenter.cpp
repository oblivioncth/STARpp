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
    QString winnerStr = result.winners().size() > 1 ?
                        WINNER_MULTI_TEMPLATE.arg(result.winners().join(R"(", ")")) :
                        WINNER_SINGLE_TEMPLATE.arg(result.winners().front());
    QString runnerUpStr = result.winners().size() > 1 ?
                          RUNNERUP_MULTI_TEMPLATE.arg(result.runnerUps().join(R"(", ")")) :
                          RUNNERUP_SINGLE_TEMPLATE.arg(result.runnerUps().front());

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
    cout << winnerStr << endl << runnerUpStr << endl << endl;

    // Print raw scores
    for(const QString& nom : nominees)
        cout << RAW_SCORE_TEMPLATE.arg(nom).arg(result.election()->totalScore(nom)) << endl;
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
        QString summaryEntry = SUMMARY_ITEM.arg(res.winners().join(R"(", ")"), res.runnerUps().join(R"(", ")"));

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
