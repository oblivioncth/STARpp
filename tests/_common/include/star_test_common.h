#include <star/election.h>
#include <star/reference.h>

// Macros
#define C_STR(q_str) q_str.toStdString().c_str()

// Test debug helpers (enables more helpful print-outs from test failures)
namespace Star
{

char* toString(const ExpectedElectionResult& eer)
{
    QString string = "W = {" + eer.winners().join(", ") +  "}  | R = {" + eer.runnerUps().join(", ") + '}';
    return qstrdup(string.toUtf8().constData());
}

char* toString(const ElectionResult& er)
{
    QString string = "W = {" + er.winners().join(", ") +  "}  | R = {" + er.runnerUps().join(", ") + '}';
    return qstrdup(string.toUtf8().constData());
}

}
