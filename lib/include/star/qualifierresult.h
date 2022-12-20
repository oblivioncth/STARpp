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

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    QualifierResult();
    QualifierResult(const QString& f, const QString& s, const QSet<QString>& o);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;
    bool isComplete() const;

    QString firstSeed() const;
    QString secondSeed() const;
    std::pair<QString, QString> seeds();
    QSet<QString> overflow() const;

    bool operator==(const QualifierResult& other) const;
    bool operator!=(const QualifierResult& other) const;
};

}

#endif // QUALIFIERRESULT_H
