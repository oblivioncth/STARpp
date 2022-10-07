#include <QCoreApplication>

// Core Includes
#include "starcalc/reference.h"

int main(int argc, char *argv[])
{
    // Just dummy code for now
    QCoreApplication a(argc, argv);

    QString ccPath = "C:/Users/Chris/Desktop/test_config.ini";
    QString bbPath = "C:/Users/Chris/Desktop/test_box.csv";

    QList<Election> elections;
    Qx::GenericError electionLoadError = electionsFromReferenceInput(elections, ccPath, bbPath);
    qDebug() << electionLoadError.isValid();

    return 0;
}
