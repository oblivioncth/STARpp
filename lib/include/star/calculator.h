#ifndef CALCULATOR_H
#define CALCULATOR_H

// Qt Includes
#include <QObject>

// Project Includes
#include "star/election.h"
#include "star/electionresult.h"

namespace Star
{

class Calculator : public QObject
{
    Q_OBJECT
//-Instance Variables--------------------------------------------------------------------------------------------------
private:

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    Calculator();

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    ElectionResult calculateResult(const Election& election);

//-Signals & Slots-------------------------------------------------------------------------------------------------
signals:
    void calculationDetailDetermined(const QString& detail);
};

}

#endif // CALCULATOR_H
