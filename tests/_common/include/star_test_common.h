#include <star/election.h>
#include <star/electionresult.h>
#include <star/expectedelectionresult.h>

#include <qx/core/qx-string.h>

// Macros
#define C_STR(q_str) q_str.toStdString().c_str()

// Test debug helpers (enables more helpful print-outs from test failures)
namespace Star
{

char* toString(const ExpectedElectionResult& eer)
{
    //QString string = "W = {" + Qx::String::join(eer.winners(), ", ") +  "}  | R = {" + Qx::String::join(eer.runnerUps(), ", ") + '}';
    QString string = "Winner = " + eer.winner();
    return qstrdup(string.toUtf8().constData());
}

char* toString(const ElectionResult& er)
{
    //QString string = "W = {" + Qx::String::join(er.winners(), ", ") +  "}  | R = {" + Qx::String::join(er.runnerUps(), ", ") + '}';
    QString string = "Winner = " + er.winner();
    return qstrdup(string.toUtf8().constData());
}

}
