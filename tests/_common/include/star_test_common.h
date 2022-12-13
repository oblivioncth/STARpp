#include <star/election.h>
#include <star/electionresult.h>
#include <star/expectedelectionresult.h>

#include <qx/core/qx-string.h>

// Macros
#define C_STR(q_str) q_str.toStdString().c_str()

// Test debug helpers (enables more helpful print-outs from test failures)
namespace Star
{

QString elecResStr(const QStringList& winners, const QSet<QString>& unres)
{
    static const QString templ = QStringLiteral("Winners = {%1} | Unresolved = {%2}");

    QString winStr = winners.isEmpty() ? QString() : '"' + winners.join(R"(", ")") + '"';
    QString unresStr = unres.isEmpty() ? QString() : '"' + Qx::String::join(unres, R"(", ")") + '"';

    return templ.arg(winStr, unresStr);
}

char* toString(const ExpectedElectionResult& eer)
{
    QString string = elecResStr(eer.winners(), eer.unresolvedCandidates());
    return qstrdup(string.toUtf8().constData());
}

char* toString(const ElectionResult& er)
{
    QString string = elecResStr(er.winners(), er.unresolvedCandidates());
    return qstrdup(string.toUtf8().constData());
}

}
