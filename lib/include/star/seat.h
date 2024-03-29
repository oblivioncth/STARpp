#ifndef SEAT_H
#define SEAT_H

// Shared Library Support
#include "star/star_base_export.h"

// Qt Includes
#include <QObject>

// Project Includes
#include "star/qualifierresult.h"

namespace Star
{

class STAR_BASE_EXPORT Seat
{
//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    QString mWinner;
    QualifierResult mQualifierResult;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    Seat();
    Seat(const QualifierResult& qualifierResult);
    Seat(const QString& winner, const QualifierResult& qualifierResult = QualifierResult());

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isNull() const;
    bool isFilled() const;
    bool isExceptionFilled() const;
    bool isUpset() const;

    QString winner() const;
    QualifierResult qualifierResult() const;

    bool operator==(const Seat& other) const;
    bool operator!=(const Seat& other) const;
};

}

#endif // SEAT_H
