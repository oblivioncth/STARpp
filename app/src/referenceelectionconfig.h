#ifndef REFERENCE_ELECTION_CONFIG_H
#define REFERENCE_ELECTION_CONFIG_H

// Qt Includes
#include <QString>

// Base Includes
#include "star/calculator.h"

struct ReferenceElectionConfig
{
    QString ccPath;
    QString bbPath;
    std::optional<Star::Calculator::ExtendedTiebreakMethod> extraTiebreakMethod;
};

#endif // REFERENCE_ELECTION_CONFIG_H
