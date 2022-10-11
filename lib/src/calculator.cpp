// Unit Include
#include "star/calculator.h"

namespace Star
{

//===============================================================================================================
// Calculator
//===============================================================================================================

//-Constructor---------------------------------------------------------------------------------------------------------
//Public:
Calculator::Calculator(bool extraTiebreak) :
    mExtraTiebreak(extraTiebreak)
{}

//-Instance Functions-------------------------------------------------------------------------------------------------
//Public:
bool Calculator::isExtraTiebreak() const { return mExtraTiebreak; }
void Calculator::setExtraTiebreak(bool extraTiebreak) { mExtraTiebreak = extraTiebreak; }

ElectionResult Calculator::calculateResult(const Election& election)
{

    return {};
}


}
