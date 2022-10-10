#ifndef REFERENCE_ELECTION_CONFIG_H
#define REFERENCE_ELECTION_CONFIG_H

// Qt Includes
#include <QString>

struct ReferenceElectionConfig
{
    QString ccPath;
    QString bbPath;
    bool extraTiebreak = false;
};

#endif // REFERENCE_ELECTION_CONFIG_H
