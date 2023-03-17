#ifndef QUALIFIERRESULT_H
#define QUALIFIERRESULT_H

// Shared Library Support
#include "star/star_base_export.h"

// Qt Includes
#include <QSet>
#include <QString>

namespace Star
{

/* TODO: Add an enum that is used for first and second seed that indicates how deep into the runoff qualifier
 * process Calculator needed to go to fill both
 */

class STAR_BASE_EXPORT QualifierResult
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QString mFirstSeed;
    QString mSecondSeed;
    QSet<QString> mOverflow;
    bool mSimultaneous;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    QualifierResult();
    QualifierResult(const QSet<QString>& firstAdv, const QSet<QString>& secondAdv = {});
    QualifierResult(const QString& f, const QString& s, bool sim, const QSet<QString>& o);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;
    bool isComplete() const;
    bool isSeededSimultaneously() const;
    bool hasFirstSeed() const;
    bool hasSecondSeed() const;

    QString firstSeed() const;
    QString secondSeed() const;
    std::pair<QString, QString> seeds();
    QSet<QString> overflow() const;
    QSet<QString> unresolved() const;

    bool operator==(const QualifierResult& other) const;
    bool operator!=(const QualifierResult& other) const;
};

}

#endif // QUALIFIERRESULT_H
