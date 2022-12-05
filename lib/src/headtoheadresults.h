#ifndef HEADTOHEADRESULTS_H
#define HEADTOHEADRESULTS_H

// Qt Includes
#include <QSet>
#include <QHash>
#include <QString>

// Qx Includes
#include <qx/core/qx-cumulation.h>

namespace Star
{

// Forward Declarations
class Election;

class HeadToHeadResults
{
//-Class Enums-----------------------------------------------------------------------------------------------------
public:
    enum NarrowMode{ Inclusive, Exclusive };

//-Inner Structs----------------------------------------------------------------------------------------------------
private:
    struct CandidateStats
    {
        QSet<QString> victories;
        QSet<QString> defeats;
        Qx::Cumulation<QString, int> preferences;
    };

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QHash<QString, CandidateStats> mStats;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    HeadToHeadResults(const Election* election);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    int wins(const QString& candidate) const;
    int losses(const QString& candidate) const;
    int preferences(const QString& candidate) const;
    int margin(const QString& candidate) const;
    QString winner(const QString& candidateA, const QString& candidateB) const;

    void narrow(QSet<QString> candidates, NarrowMode mode);
};

}

#endif // HEADTOHEADRESULTS_H
