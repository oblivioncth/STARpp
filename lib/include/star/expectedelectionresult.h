#ifndef EXPECTEDELECTIONRESULT_H
#define EXPECTEDELECTIONRESULT_H

// Qt Includes
#include <QStringList>

// Project Includes
#include "star/electionresult.h"

namespace Star
{

class ExpectedElectionResult
{
//-Inner Classes----------------------------------------------------------------------------------------------------
public:
    class Builder;

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QList<Seat> mSeats;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ExpectedElectionResult();

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;
    bool sameOutcomeAs(const ElectionResult& result);

    qsizetype seatCount() const;
    QList<Seat> seats() const;

    bool operator==(const ElectionResult& result) const;
    bool operator!=(const ElectionResult& result) const;
};

class ExpectedElectionResult::Builder
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    ExpectedElectionResult mConstruct;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    Builder();

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    Builder& wSeat(const Seat& seat);
    Builder& wWinner(const QString& winner);

    ExpectedElectionResult build();
};

}

#endif // EXPECTEDELECTIONRESULT_H
