// Unit Include
#include "star/qualifierresult.h"

namespace Star
{

//===============================================================================================================
// QualifierResult
//===============================================================================================================

/*!
 *  @class QualifierResult star/qualifierresult.h
 *
 *  @brief The QualifierResult class holds the outcome of a runoff qualifier.
 *
 *  When filling a seat in a STAR election, two candidates from the total pool are seeded into the runoff round
 *  before the final winner is determined, a process that STAR++ has dubbed the "Runoff Qualifier". A given
 *  instance of this class contains information about the result of one of these qualifiers for a single seat.
 *  This information can be useful for inspecting the details of the STAR process, how close a given election
 *  was, and whether the strength of votes or consensus were the larger deciding factor for a particular winner.
 *
 *  @sa ElectionResult.
 */

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
/*!
 *  Constructs a null qualifier result.
 *
 *  @sa isNull().
 */
QualifierResult::QualifierResult() :
    mFirstSeed(),
    mSecondSeed(),
    mOverflow(),
    mSimultaneous(false)
{}

/*!
 *  Constructs a qualifier result with first seed @a f, second seed @a s, overflow @a o, and the
 *  simultaneously seeded indicator @a sim.
 *
 *  @note
 *  If the arguments passed to this constructor constitute an invalid qualifier result, the resultant
 *  instance will be null instead.
 */
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

/*!
 *  Constructs a qualifier result in which the candidate(s) in @a firstAdv were advanced first, and the
 *  candidate(s) in @a secondAdv were advanced second.
 *
 *  @note
 *  If the arguments passed to this constructor constitute an invalid qualifier result, the resultant
 *  instance will be null instead.
 */
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
/*!
 *  Returns @c true if the qualifier result is null; otherwise, returns @c false.
 *
 *  @sa isComplete().
 */
bool QualifierResult::isNull() const { return mFirstSeed.isNull() && mSecondSeed.isNull() && mOverflow.isEmpty(); }

/*!
 *  Returns @c true if the qualifier successfully seeded two candidates for a runoff;
 *  otherwise, returns false.
 *
 *  @sa isNull() and Calculator::Option::AllowTrueTies.
 */
bool QualifierResult::isComplete() const { return !mFirstSeed.isEmpty() && !mSecondSeed.isEmpty() && mOverflow.isEmpty(); }

/*!
 *  Returns @c true if both of the seeded candidates were advanced at the same time;
 *  otherwise, returns false.
 */
bool QualifierResult::isSeededSimultaneously() const { return mSimultaneous; }

/*!
 *  Returns @c true if the qualifier successfully selected a first seed;
 *  otherwise, returns false.
 *
 *  @sa isComplete() and Calculator::Option::AllowTrueTies.
 */
bool QualifierResult::hasFirstSeed() const { return !mFirstSeed.isNull(); }

/*!
 *  Returns @c true if the qualifier successfully selected a second seed;
 *  otherwise, returns false.
 *
 *  @sa isComplete() and Calculator::Option::AllowTrueTies.
 */
bool QualifierResult::hasSecondSeed() const { return !mSecondSeed.isNull(); }

/*!
 *  Returns the first candidate seeded into the runoff that this qualifier result
 *  details.
 *
 *  @sa hasFirstSeed() and secondSeed().
 */
QString QualifierResult::firstSeed() const { return mFirstSeed; }

/*!
 *  Returns the first candidate seeded into the runoff that this qualifier result
 *  details.
 *
 *  @sa hasFirstSeed() and secondSeed().
 */
QString QualifierResult::secondSeed() const { return mSecondSeed; }

/*!
 *  Returns the first and second runoff seeds as a pair.
 *
 *  @sa firstSeed() and secondSeed().
 */
std::pair<QString, QString> QualifierResult::seeds() { return std::make_pair(mFirstSeed, mSecondSeed); }

/*!
 *  Returns the set of candidates stuck in an unresolvable tie, if the qualifier did not
 *  successfully seed both candidates; otherwise, returns an empty set.
 *
 *  @sa isComplete() and unresolved().
 */
QSet<QString> QualifierResult::overflow() const { return mOverflow; }

/*!
 *  Returns the set of candidates remaining in the qualifier when it failed, if it is incomplete.
 *  Otherwise, returns an empty set.
 *
 *  Unlike overflow(), this method will include the first seed if one was successfully seeded.
 *  seeded.
 *
 *  @sa isComplete() and overflow().
 */
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

/*!
 *  Returns true if this qualifier result is the same as @a other; otherwise, returns false.
 *
 *  When evaluating if two qualifier results are equivalent, the order of the candidate
 *  seeds is ignored if both were seeded simultaneously.
 *
 *  @sa operator!=().
 */
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

/*!
 *  Returns true if this qualifier result is not the same as @a other; otherwise, returns false.
 *
 *  When evaluating if two qualifier results are not equivalent, the order of the candidate
 *  seeds is ignored if both were seeded simultaneously.
 *
 *  @sa operator==().
 */
bool QualifierResult::operator!=(const QualifierResult& result) const { return !(*this == result); }

}
