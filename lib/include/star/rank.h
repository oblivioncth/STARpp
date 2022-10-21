#ifndef RANK_H
#define RANK_H

// Qt Includes
#include <QStringList>

struct Rank
{
    uint value;
    QStringList nominees;

    static QList<Rank> rankSort(const QMap<QString, uint>& valueMap);
};

#endif // RANK_H
