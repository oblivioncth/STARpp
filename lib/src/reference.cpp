// Unit Include
#include "starcalc/reference.h"
#include "reference/categoryconfig_p.h"
#include "reference/ballotbox_p.h"

namespace Star
{

namespace
{
    QList<Election> electionTransform(const RefBallotBox& box)
    {
        QList<Election> views;

        // Create an election for each category
        for(qsizetype cIdx = 0; cIdx < box.categories().size(); cIdx++)
        {
            // Get current category
            const RefCategory& rCategory = box.categories()[cIdx];

            // Initialize election builder
            Election::Builder eBuilder(rCategory.name);

            // Create category specific ballots
            for(qsizetype bIdx = 0; bIdx < box.ballots().size(); bIdx++)
            {
                // Get current ballot
                const RefBallot& rBallot = box.ballots()[bIdx];

                // Get votes that pertain to category
                const QList<uint>& categoryVotes = rBallot.votes[cIdx];

                // List to fill with nominee mapped votes in standard form
                QList<Vote> mappedVotes;

                // Map votes to nominees, convert to standard Vote, add to list
                for(qsizetype nIdx = 0; nIdx < categoryVotes.size(); nIdx++)
                {
                    Vote stdVote{.nominee = rCategory.nominees[nIdx], .score = categoryVotes[nIdx]};
                    mappedVotes.append(stdVote);
                }

                // Create standard voter (For now, just set the anonymous name to "Voter N")
                static const QString anonTemplate = QStringLiteral("Voter %1");
                Voter stdVoter{.name = rBallot.voter, .anonymousName = anonTemplate.arg(bIdx)};

                // Add ballot to builder
                eBuilder.wBallot(stdVoter, mappedVotes);
            }

            // Build election and add to list
            views.append(eBuilder.build());
        }

        return views;
    }
}

//-Namespace-Functions--------------------------------------------------------------------------------
Qx::GenericError electionsFromReferenceInput(QList<Election>& returnBuffer,
                                            const QString& categoryConfigPath,
                                            const QString& ballotBoxPath)
{
    // Clear return buffer
    returnBuffer.clear();

    // Status tracker
    Qx::GenericError errorStatus;

    // Read category config
    RefCategoryConfig cc;
    RefCategoryConfig::Reader ccReader(&cc, categoryConfigPath);
    if((errorStatus = ccReader.readInto()).isValid())
        return errorStatus;

    // Read ballot box
    RefBallotBox bb;
    RefBallotBox::Reader bbReader(&bb, ballotBoxPath, &cc);
    if((errorStatus = bbReader.readInto()).isValid())
        return errorStatus;

    // Create elections from standard ballot box
    returnBuffer = electionTransform(bb);

    return errorStatus;
}

}
