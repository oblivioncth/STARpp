#ifndef RANK_H
#define RANK_H

// Shared Library Support
#include "star/star_base_export.h"

// Qt Includes
#include <QSet>

namespace Star
{

struct STAR_BASE_EXPORT Rank
{
    enum Order { Ascending, Descending };

    int value;
    QSet<QString> candidates;

    static QList<Rank> rankSort(const QMap<QString, int>& valueMap, Order order = Descending);
};

}

#endif // RANK_H
