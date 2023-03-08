#ifndef REFERENCE_H
#define REFERENCE_H

// Shared Library Support
#include "star/star_base_export.h"

// Project Includes
#include "star/election.h"
#include "star/expectedelectionresult.h"
#include "star/calculator.h"

namespace Star
{

enum class ReferenceErrorType { NoError, CategoryConfig, BallotBox, ExpectedResult, CalcOptions };

struct ReferenceError
{
    ReferenceErrorType type = ReferenceErrorType::NoError;
    QString error;
    QString errorDetails;

    bool isValid() { return type != ReferenceErrorType::NoError; }
};

//-Namespace-Functions--------------------------------------------------------------------------------
STAR_BASE_EXPORT ReferenceError electionsFromReferenceInput(QList<Election>& returnBuffer,
                                                            const QString& categoryConfigPath,
                                                            const QString& ballotBoxPath);

STAR_BASE_EXPORT ReferenceError expectedResultsFromReferenceInput(QList<ExpectedElectionResult>& returnBuffer,
                                                                  const QString& resultSetPath);

STAR_BASE_EXPORT ReferenceError calculatorOptionsFromReferenceInput(Star::Calculator::Options& returnBuffer,
                                                                    const QString& calcOptionsPath);
}

#endif // REFERENCE_H
