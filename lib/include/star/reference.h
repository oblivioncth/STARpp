#ifndef REFERENCE_H
#define REFERENCE_H

// Qx Includes
#include <qx/core/qx-genericerror.h>

// Project Includes
#include "star/election.h"
#include "star/expectedelectionresult.h"

namespace Star
{

//-Namespace-Functions--------------------------------------------------------------------------------
Qx::GenericError electionsFromReferenceInput(QList<Election>& returnBuffer,
                                             const QString& categoryConfigPath,
                                             const QString& ballotBoxPath);

Qx::GenericError expectedResultsFromReferenceInput(QList<ExpectedElectionResult>& returnBuffer,
                                                   const QString& resultSetPath);

}

#endif // REFERENCE_H
