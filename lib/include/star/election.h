#ifndef ELECTION_H
#define ELECTION_H

// Qt Includes
#include "qdatetime.h"
#include <QString>
#include <QList>

namespace Star
{

class Election
{
//-Inner Classes----------------------------------------------------------------------------------------------------
public:
    struct Rank;
    struct Vote;
    struct Voter;
    class Ballot;
    class Builder;

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QString mName;
    QList<Ballot> mBallots;
    QMap<QString, uint> mTotals;
    QList<Rank> mRankings;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    Election();

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    QString name() const;
    QStringList nominees() const;
    const QList<Ballot>& ballots() const;

    uint totalScore(const QString& nominee) const;
    const QList<Rank>& rankings() const;
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


struct Election::Rank
{
    uint totalScore;
    QList<QString> nominees;
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

//-Class Functions-------------------------------------------------------------------------------------------------
private:
   static QList<Rank> formRankings(const QMap<QString, uint>& totalScores);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    Builder& wBallot(const Voter& voter, const QList<Vote>& votes);
    Election build();
};

}

#endif // ELECTION_H
