#ifndef RANK_H
#define RANK_H

// Qt Includes
#include <QSet>

struct Rank
{
    int value;
    QSet<QString> nominees;

    static QList<Rank> rankSort(const QMap<QString, int>& valueMap);
};

#endif // RANK_H
