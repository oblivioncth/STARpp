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
//-Class Variables------------------------------------------------------------------------------------------------------
private:
    static inline const QString LOG_EVENT_ = QStringLiteral();

//-Instance Variables--------------------------------------------------------------------------------------------------
private:
    bool mExtraTiebreak;

//-Constructor---------------------------------------------------------------------------------------------------------
public:
    Calculator(bool extraTiebreak = false);

//-Instance Functions-------------------------------------------------------------------------------------------------
public:
    bool isExtraTiebreak() const;
    void setExtraTiebreak(bool extraTiebreak);

    ElectionResult calculateResult(const Election& election);

//-Signals & Slots-------------------------------------------------------------------------------------------------
signals:
    void calculationDetailDetermined(const QString& detail);
};

}

#endif // CALCULATOR_H
