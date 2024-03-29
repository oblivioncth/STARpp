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
/*! @cond */
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
        Qx::Cumulation<QString, int> antiPreferences;
    };

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QHash<QString, CandidateStats> mStats;

//-Constructor---------------------------------------------------------------------------------------------------------
private:
    HeadToHeadResults();

public:
    HeadToHeadResults(const Election* election);

//-Instance Functions-------------------------------------------------------------------------------------------------
private:
    void faceOffStatsUpdate(const QString& can, int canPref, const QString& opp, int oppPref);

public:
    int wins(const QString& candidate) const;
    int losses(const QString& candidate) const;
    int preferences(const QString& candidate) const;
    int margin(const QString& candidate) const;
    QString winner(const QString& candidateA, const QString& candidateB) const;

    qsizetype candidateCount() const;
    QSet<QString> candidates() const;

    void narrow(QSet<QString> candidates, NarrowMode mode);
    HeadToHeadResults narrowed(QSet<QString> candidates, NarrowMode mode);
};
/*! @endcond */
}

#endif // HEADTOHEADRESULTS_H
