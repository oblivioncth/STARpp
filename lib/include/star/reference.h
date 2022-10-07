#ifndef REFERENCE_H
#define REFERENCE_H

// Qx Includes
#include <qx/core/qx-genericerror.h>

// Project Includes
#include "star/election.h"

namespace Star
{

//-Namespace-Functions--------------------------------------------------------------------------------
Qx::GenericError electionsFromReferenceInput(QList<Election>& returnBuffer,
                                             const QString& categoryConfigPath,
                                             const QString& ballotBoxPath);

}

#endif // REFERENCE_H
