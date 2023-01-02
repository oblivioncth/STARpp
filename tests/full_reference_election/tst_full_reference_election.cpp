// Qt Includes
#include <QtTest>

// Base Includes
#include <star/reference.h>
#include <star/calculator.h>

// Test Includes
#include <star_test_common.h>

class tst_full_reference_election : public QObject
{
    Q_OBJECT

public:
    tst_full_reference_election();

private slots:
    // Init
//    void initTestCase();
//    void cleanupTestCase();

    // Test cases
    void various_ref_elections_data();
    void various_ref_elections();

};

tst_full_reference_election::tst_full_reference_election() {}
//void tst_full_reference_election::initTestCase() {}
//void tst_full_reference_election::cleanupTestCase() {}

void tst_full_reference_election::various_ref_elections_data()
{
    // Setup test table
    QTest::addColumn<QString>("bb_path");
    QTest::addColumn<QString>("cc_path");
    QTest::addColumn<QString>("er_path");
    QTest::addColumn<QString>("op_path");

    // Populate test table rows from file
    QDir data(":/data");
    QFileInfoList dataFiles = data.entryInfoList(QDir::NoFilter, QDir::Name);
    QVERIFY(dataFiles.size() % 4 == 0);

    qsizetype testSets = dataFiles.size()/4;
    for(qsizetype i = 0; i < testSets; i++)
    {
        qsizetype fileStart = i * 4;
        const QFileInfo& bbFile = dataFiles[fileStart];
        const QFileInfo& ccFile = dataFiles[fileStart + 1];
        const QFileInfo& erFile = dataFiles[fileStart + 2];
        const QFileInfo& opFile = dataFiles[fileStart + 3];

        QVERIFY(bbFile.baseName() == ccFile.baseName() && bbFile.baseName() == erFile.baseName() && bbFile.baseName() == opFile.baseName());
        QVERIFY(bbFile.suffix() == "csv" && ccFile.suffix() == "ini" && erFile.suffix() == "json" && opFile.suffix() == "opt");

        QTest::newRow(C_STR(bbFile.baseName())) << bbFile.filePath() << ccFile.filePath() << erFile.filePath() << opFile.filePath();
    }
}

void tst_full_reference_election::various_ref_elections()
{
    // Fetch data from test table
    QFETCH(QString, bb_path);
    QFETCH(QString, cc_path);
    QFETCH(QString, er_path);
    QFETCH(QString, op_path);

    // Load expected results
    QList<Star::ExpectedElectionResult> expectedResults;
    Star::ReferenceError expectedResultsLoadError = Star::expectedResultsFromReferenceInput(expectedResults, er_path);
    QVERIFY2(!expectedResultsLoadError.isValid(), expectedResultsLoadError.errorDetails.toStdString().c_str());

    // Load reference elections
    QList<Star::Election> elections;
    Star::ReferenceError electionLoadError = Star::electionsFromReferenceInput(elections, cc_path, bb_path);
    QVERIFY2(!electionLoadError.isValid(), electionLoadError.errorDetails.toStdString().c_str());

    // Load options
    Star::Calculator::Options cOptions;
    Star::ReferenceError calcOptionsLoadError = Star::calculatorOptionsFromReferenceInput(cOptions, op_path);
    QVERIFY2(!calcOptionsLoadError.isValid(), calcOptionsLoadError.errorDetails.toStdString().c_str());

    // Create calculator
    Star::Calculator calculator;
    calculator.setOptions(cOptions);

    // Check the results of each election
    for(qsizetype i = 0; i < elections.size(); i++)
    {
        const Star::Election& election = elections.at(i);
        calculator.setElection(&election);
        Star::ElectionResult result = calculator.calculateResult();
        QCOMPARE(result, expectedResults.at(i));
    }
}

QTEST_APPLESS_MAIN(tst_full_reference_election)
#include "tst_full_reference_election.moc"
