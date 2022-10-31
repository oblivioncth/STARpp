#ifndef ELECTION_H
#define ELECTION_H

// Qt Includes
#include "qdatetime.h"
#include <QString>
#include <QList>

// Project Includes
#include "star/rank.h"

namespace Star
{

class Election
{
//-Inner Classes----------------------------------------------------------------------------------------------------
public:
    struct Vote;
    struct Voter;
    class Ballot;
    class Builder;

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QString mName;
    QList<Ballot> mBallots;
    QMap<QString, uint> mTotals;
    QList<Rank> mScoreRankings;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    Election();

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isValid() const;

    QString name() const;
    QStringList nominees() const;
    const QList<Ballot>& ballots() const;

    uint totalScore(const QString& nominee) const;
    const QList<Rank>& scoreRankings() const;
};

struct Election::Voter
{
    QString name;
    QString anonymousName;
};

struct Election::Vote
{
    QString nominee;
    uint score = 0;
};

class Election::Ballot
{
    friend class Election::Builder;
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    Voter mVoter;
    QHash<QString, uint> mVotes;

//-Constructor--------------------------------------------------------------------------------------------------------
private:
    Ballot();

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    const Voter& voter() const;

    uint score(const QString& nominee) const;
    QString preference(const QStringList& nominees) const;
};

class Election::Builder
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    Election mConstruct;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    Builder(const QString& name);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    Builder& wBallot(const Voter& voter, const QList<Vote>& votes);
    void reset();
    Election build();
};

}

#endif // ELECTION_H
