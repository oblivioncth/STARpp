#include <QCoreApplication>

// Core Includes
#include "star/reference.h"

int main(int argc, char *argv[])
{
    // Just dummy code for now
    QCoreApplication a(argc, argv);

    QString ccPath = "C:/Users/Chris/Desktop/test_config.ini";
    QString bbPath = "C:/Users/Chris/Desktop/test_box.csv";

    QList<Star::Election> elections;
    Qx::GenericError electionLoadError = Star::electionsFromReferenceInput(elections, ccPath, bbPath);
    qDebug() << electionLoadError.isValid();

    return 0;
}
