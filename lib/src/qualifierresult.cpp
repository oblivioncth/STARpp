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
    mOverflow(),
    mSimultaneous(false)
{}

QualifierResult::QualifierResult(const QString& f, const QString& s, bool sim, const QSet<QString>& o)
{
    // Cover all valid qualifier outcome types
    if((f.isNull() && s.isNull() && !sim && o.size() > 2) || // Unresolvable tie for 1st seed
       (!f.isNull() && s.isNull() && !sim && o.size() > 1) || // Unresolvable tie for 2nd seed
       (!f.isNull() && !s.isNull() && o.isEmpty())) // Simultaneous or separate fill of both seeds
    {
        mFirstSeed = f;
        mSecondSeed = s;
        mSimultaneous = sim;
        mOverflow = o;
    }

    // Leave instance null if the input was invalid
}

QualifierResult::QualifierResult(const QSet<QString>& firstAdv, const QSet<QString>& secondAdv) :
    mSimultaneous(false)
{
    // Cover all valid qualifier outcome types
    if(firstAdv.size() == 2 && secondAdv.isEmpty()) // Simultaneous fill of both seeds
    {
        auto fItr = firstAdv.cbegin();
        mFirstSeed = *(fItr++);
        mSecondSeed = *fItr;
        mSimultaneous = true;
    }
    else if(firstAdv.size() > 2 && secondAdv.isEmpty()) // Unresolvable tie for 1st seed
        mOverflow = firstAdv;
    else if(firstAdv.size() == 1 && secondAdv.size() > 1) // Unresolvable tie for 2nd seed
    {
        mFirstSeed = *firstAdv.cbegin();
        mOverflow = secondAdv;
    }
    else if(firstAdv.size() == 1 && secondAdv.size() == 1) // Separate fill of both seeds
    {
        mFirstSeed = *firstAdv.cbegin();
        mSecondSeed = *secondAdv.cbegin();
    }

    // Leave instance null if the input was invalid
}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool QualifierResult::isNull() const { return mFirstSeed.isNull() && mSecondSeed.isNull() && mOverflow.isEmpty(); }
bool QualifierResult::isComplete() const { return !mFirstSeed.isEmpty() && !mSecondSeed.isEmpty() && mOverflow.isEmpty();}
bool QualifierResult::isSeededSimultaneously() const { return mSimultaneous; }
bool QualifierResult::hasFirstSeed() const { return !mFirstSeed.isNull(); }
bool QualifierResult::hasSecondSeed() const { return !mSecondSeed.isNull(); }

QString QualifierResult::firstSeed() const { return mFirstSeed; }
QString QualifierResult::secondSeed() const { return mSecondSeed; }
std::pair<QString, QString> QualifierResult::seeds() { return std::make_pair(mFirstSeed, mSecondSeed); }
QSet<QString> QualifierResult::overflow() const { return mOverflow; }

QSet<QString> QualifierResult::unresolved() const
{
    if(isNull() || isComplete())
        return QSet<QString>();
    else
    {
        QSet<QString> unr = mOverflow;
        if(!mFirstSeed.isNull())
            unr.insert(mFirstSeed);

        return unr;
    }
}

bool QualifierResult::operator==(const QualifierResult& result) const
{
    if(mOverflow != result.overflow() || mSimultaneous != result.isSeededSimultaneously())
        return false;

    if(mSimultaneous)
    {
        return (result.firstSeed() == mFirstSeed && result.secondSeed() == mSecondSeed) ||
               (result.firstSeed() == mSecondSeed && result.secondSeed() == mFirstSeed);
    }
    else
        return result.firstSeed() == mFirstSeed && result.secondSeed() == mSecondSeed;
}

bool QualifierResult::operator!=(const QualifierResult& result) const { return !(*this == result); }

}
