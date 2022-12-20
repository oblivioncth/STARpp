// Unit Include
#include "star/qualifierresult.h"

namespace Star
{

//===============================================================================================================
// QualifierResult
//===============================================================================================================

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
QualifierResult::QualifierResult() :
    mFirstSeed(),
    mSecondSeed(),
    mOverflow()
{}

QualifierResult::QualifierResult(const QString& f, const QString& s, const QSet<QString>& o) :
    mFirstSeed(f),
    mSecondSeed(s),
    mOverflow(o)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool QualifierResult::isNull() const { return mFirstSeed.isNull() && mSecondSeed.isNull() && mOverflow.isEmpty(); }
bool QualifierResult::isComplete() const { return !mFirstSeed.isEmpty() && !mSecondSeed.isEmpty() && mOverflow.isEmpty();}

QString QualifierResult::firstSeed() const { return mFirstSeed; }
QString QualifierResult::secondSeed() const { return mSecondSeed; }
std::pair<QString, QString> QualifierResult::seeds() { return std::make_pair(mFirstSeed, mSecondSeed); }
QSet<QString> QualifierResult::overflow() const { return mOverflow; }

bool QualifierResult::operator==(const QualifierResult& result) const
{
    return result.firstSeed() == mFirstSeed && result.secondSeed() == mSecondSeed && result.overflow() == mOverflow;
}

bool QualifierResult::operator!=(const QualifierResult& result) const { return !(*this == result); }

}
