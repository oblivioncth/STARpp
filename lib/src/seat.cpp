// Unit Include
#include "star/seat.h"

namespace Star
{

//===============================================================================================================
// Seat
//===============================================================================================================

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
Seat::Seat() :
    mWinner(),
    mQualifierResult()
{}

Seat::Seat(const QualifierResult& qualifierResult) :
    mWinner(),
    mQualifierResult(qualifierResult)
{}

Seat::Seat(const QString& winner, const QualifierResult& qualifierResult) :
    mWinner(winner),
    mQualifierResult(qualifierResult)
{}


//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool Seat::isNull() const { return mWinner.isNull() && mQualifierResult.isNull(); }
bool Seat::isFilled() const { return !mWinner.isNull(); }
bool Seat::isExceptionFilled() const { return !isNull() && mQualifierResult.isNull(); } // NOTE: Filled without runoff

QString Seat::winner() const { return mWinner; }
QualifierResult Seat::qualifierResult() const { return mQualifierResult; }

bool Seat::operator==(const Seat& other) const
{
    return mWinner == other.winner() && mQualifierResult == other.qualifierResult();
}

bool Seat::operator!=(const Seat& other) const { return !(*this == other); }

}
