#ifndef ELECTIONRESULT_H
#define ELECTIONRESULT_H

// Shared Library Support
#include "star/star_base_export.h"

// Project Includes
#include "star/election.h"
#include "star/seat.h"

namespace Star
{

class STAR_BASE_EXPORT ElectionResult
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    const Election* mElection;
    QList<Seat> mSeats;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    ElectionResult();
    ElectionResult(const Election* election, const QList<Seat>& seats);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;
    bool isComplete() const;

    const Seat& seatAt(qsizetype i) const;
    QList<Seat> seats() const;
    QStringList winners() const;
    QSet<QString> unresolvedCandidates() const;
    Seat unresolvedSeat() const;
    qsizetype seatCount() const;
    qsizetype filledSeatCount() const;
    qsizetype unfilledSeatCount() const;
    const Election* election() const;

    bool operator==(const ElectionResult& other) const;
    bool operator!=(const ElectionResult& other) const;
};

}

#endif // ELECTIONRESULT_H
