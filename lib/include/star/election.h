#ifndef ELECTION_H
#define ELECTION_H

// Shared Library Support
#include "star/star_base_export.h"

// Qt Includes
#include "qdatetime.h"
#include <QString>
#include <QList>

// Project Includes
#include "star/rank.h"

namespace Star
{

class STAR_BASE_EXPORT Election
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
    QStringList candidates() const;
    const QList<Ballot>& ballots() const;
    int seatCount() const;

    int totalScore(const QString& candidate) const;
    const QList<Rank>& scoreRankings() const;
};

struct Election::Voter
{
    QString name;
    QString anonymousName;
};

struct Election::Vote
{
    QString candidate;
    int score = 0;
};

class STAR_BASE_EXPORT Election::Ballot
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

    int score(const QString& candidate) const;
    QString preference(const QString& candidateA, const QString& candidateB) const;
};

class STAR_BASE_EXPORT Election::Builder
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    Election mConstruct;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    Builder(const QString& name = QString());

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    Builder& wName(const QString& name);
    Builder& wBallot(const Voter& voter, const QList<Vote>& votes);
    Builder& wSeatCount(int count);
    void reset();
    Election build();
};

}

#endif // ELECTION_H
