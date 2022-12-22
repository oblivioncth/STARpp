#ifndef QUALIFIERRESULT_H
#define QUALIFIERRESULT_H

// Qt Includes
#include <QSet>
#include <QString>

namespace Star
{

class QualifierResult
{
friend class Calculator;

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
