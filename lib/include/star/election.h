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
    int mSeats;
    QMap<QString, int> mTotals;
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
    int seatCount() const;

    int totalScore(const QString& nominee) const;
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
    int score = 0;
};

class Election::Ballot
{
    friend class Election::Builder;
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    Voter mVoter;
    QHash<QString, int> mVotes;

//-Constructor--------------------------------------------------------------------------------------------------------
private:
    Ballot();

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    const Voter& voter() const;

    int score(const QString& nominee) const;
    QString preference(const QString& nomineeA, const QString& nomineeB) const;
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
    Builder& wSeatCount(int count);
    void reset();
    Election build();
};

}

#endif // ELECTION_H
