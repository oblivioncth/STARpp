// Unit Include
#include "core.h"

// Qt Includes
#include <QCoreApplication>

// Qx Includes
#include <qx/core/qx-iostream.h>
#include <qx/utility/qx-helpers.h>

//===============================================================================================================
// CORE
//===============================================================================================================

//-Constructor-------------------------------------------------------------
Core::Core(QCoreApplication* app) :
    QObject(app),
    mLogger(QFileInfo(QCoreApplication::applicationFilePath()).baseName() + '.' + LOG_FILE_EXT, app),
    mLogErrorOccurred(false),
    mArguments(app->arguments()),
    mRefElectionCfg(std::nullopt)
{
    // Logger tweaks
    mLogger.setMaximumEntries(50);
}

//-Instance Functions-------------------------------------------------------------
//Private:
void Core::handleLogError(Qx::IoOpReport error)
{
    // Note error
    mLogErrorOccurred = true;

    // Post error
    static Qx::GenericError errorMsg(Qx::GenericError::Warning, ERR_LOG_ERROR, error.outcomeInfo());
    Qx::cout << errorMsg;
}

void Core::showHelp()
{
    // Help string
    static QString helpStr;

    // One time setup
    if(helpStr.isNull())
    {
        // Help options
        QString optStr;
        for(const QCommandLineOption* clOption : CL_OPTIONS_ALL)
        {
            // Handle names
            QStringList dashedNames;
            for(const QString& name : qxAsConst(clOption->names()))
                dashedNames << ((name.length() > 1 ? "--" : "-") + name);

            // Add option
            optStr += HELP_OPT_TEMPL.arg(dashedNames.join(" | "), clOption->description());
        }

        // Complete string
        helpStr = HELP_TEMPL.arg(optStr);
    }

    // Show help
    postMessage(helpStr);
}


void Core::showVersion()
{
    postMessage(CL_VERSION_MESSAGE);
}

//Public:
ErrorCode Core::initialize()
{
    // Open log
    Qx::IoOpReport logOpen = mLogger.openLog();
    if(logOpen.isFailure())
        postError(NAME, Qx::GenericError(Qx::GenericError::Warning, logOpen.outcome(), logOpen.outcomeInfo()));

    // Log initialization step
    logEvent(NAME, LOG_EVENT_INIT);

    // Setup CLI Parser
    QCommandLineParser clParser;
    clParser.setOptionsAfterPositionalArgumentsMode(QCommandLineParser::ParseAsPositionalArguments);
    for(const QCommandLineOption* clOption : CL_OPTIONS_ALL)
        clParser.addOption(*clOption);

    // Parse
    if(!clParser.parse(mArguments))
    {
        showHelp();
        logError(NAME, Qx::GenericError(Qx::GenericError::Error, LOG_ERR_INVALID_ARGS, clParser.errorText()));
        return ErrorCode::INVALID_ARGS;
    }

    // Handle arguments
    if(clParser.isSet(CL_OPTION_VERSION))
    {
        showVersion();
        logEvent(NAME, LOG_EVENT_VER_SHOWN);
    }
    else if(clParser.isSet(CL_OPTION_HELP) || clParser.optionNames().count() == 0) // Also when no parameters
    {
        showHelp();
        logEvent(NAME, LOG_EVENT_G_HELP_SHOWN);
    }
    else if(clParser.isSet(CL_OPTION_CONFIG) && clParser.isSet(CL_OPTION_BOX))
    {
        mRefElectionCfg = ReferenceElectionConfig{
            .ccPath = clParser.value(CL_OPTION_CONFIG),
            .bbPath = clParser.value(CL_OPTION_BOX),
            .extraTiebreak = clParser.isSet(CL_OPTION_TOP)
        };
    }
    else
    {
        logError(NAME, Qx::GenericError(Qx::GenericError::Error, LOG_ERR_INVALID_ARGS, ERR_MISSING_REF_PATHS));
        return ErrorCode::INVALID_ARGS;
    }

    // Return success
    return ErrorCode::NO_ERR;
}

bool Core::hasActionableArguments() { return mRefElectionCfg.has_value(); }

ReferenceElectionConfig Core::referenceElectionConfig()
{
    return mRefElectionCfg.has_value() ? mRefElectionCfg.value() : ReferenceElectionConfig();
}

//-Signals & Slots------------------------------------------------------------------------------------------------------------
//Public slots:
void Core::logError(QString src, Qx::GenericError error)
{
    if(!mLogErrorOccurred)
    {
        Qx::IoOpReport lr = mLogger.recordErrorEvent(src, error);
        if(lr.isFailure())
            handleLogError(lr);
    }
}

void Core::logEvent(QString src, QString event)
{
    if(!mLogErrorOccurred)
    {
        Qx::IoOpReport lr = mLogger.recordGeneralEvent(src, event);
        if(lr.isFailure())
            handleLogError(lr);
    }
}

void Core::logCalculatorDetail(QString detail) { logEvent("Calculator", detail); }

ErrorCode Core::logFinish(ErrorCode exitCode)
{
    if(!mLogErrorOccurred)
    {
        Qx::IoOpReport lr = mLogger.finish(exitCode);
        if(lr.isFailure())
            handleLogError(lr);
    }

    return exitCode;
}

void Core::postError(QString src, Qx::GenericError error)
{
    Qx::cout << error << Qt::endl;
    logError(src, error);
}

void Core::postMessage(QString msg)
{
    Qx::cout << msg << Qt::endl;
}
