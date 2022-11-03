#ifndef RANK_H
#define RANK_H

// Qt Includes
#include <QSet>

struct Rank
{
    uint value;
    QSet<QString> nominees;

    static QList<Rank> rankSort(const QMap<QString, uint>& valueMap);
};

#endif // RANK_H
