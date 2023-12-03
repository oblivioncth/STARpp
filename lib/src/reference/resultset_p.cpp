// Unit Include
#include "resultset_p.h"

// Qt Includes
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

// Qx Includes
#include <qx/io/qx-common-io.h>
#include <qx/core/qx-json.h>

/* TODO: Somehow (whether adding a way to specify class construction in Qx Json, or other)
 * to have the json data parsed directly into the results list
 */
namespace
{

struct JsonQualifer
{
    QSet<QString> firstAdv;
    QSet<QString> secondAdv;

    QX_JSON_STRUCT(
        firstAdv,
        secondAdv
    );
};

struct JsonSeat
{
    QString winner;
    JsonQualifer qualifier;

    QX_JSON_STRUCT(
        winner,
        qualifier
    );
};

}

namespace Star
{
/*! @cond */
//===============================================================================================================
// ResultSetReader
//===============================================================================================================

//-Constructor-----------------------------------------------------------------------------------------------------
//Protected:
ResultSetReader::ResultSetReader(QList<ExpectedElectionResult>* targetList, const QString& filePath) :
    mTargetList(targetList),
    mFile(filePath)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
Qx::Error ResultSetReader::readInto()
{
    // Clear return buffer
    mTargetList->clear();

    // Parse JSON from file
    QList<QList<JsonSeat>> jsonResults;
    Qx::JsonError err = Qx::parseJson(jsonResults, mFile);

    // Load results into target list
    for(const auto& res : jsonResults)
    {
        Star::ExpectedElectionResult::Builder eerb;
        for(const auto& seat : res)
            eerb.wSeat(Seat(seat.winner, QualifierResult(seat.qualifier.firstAdv, seat.qualifier.secondAdv)));
        mTargetList->append(eerb.build());
    }

    return Qx::Error();
}
/*! @endcond */
}
