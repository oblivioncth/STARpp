#ifndef ELECTIONRESULT_H
#define ELECTIONRESULT_H

// Qt Includes

// Project Includes
#include "star/election.h"

namespace Star
{

class ElectionResult
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    const Election* mElection;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ElectionResult();

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    const Election* election();
};

}

#endif // ELECTIONRESULT_H
