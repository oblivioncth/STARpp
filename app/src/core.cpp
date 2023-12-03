// Unit Include
#include "core.h"

// Qt Includes
#include <QCoreApplication>

// Qx Includes
#include <qx/core/qx-iostream.h>
#include <qx/utility/qx-helpers.h>

// Macros
#define ENUM_NAME(eenum) QString(magic_enum::enum_name(eenum).data())

//===============================================================================================================
// CoreError
//===============================================================================================================

//-Constructor-------------------------------------------------------------
//Private:
CoreError::CoreError(Type t, const QString& s, Qx::Severity sv) :
    mType(t),
    mSpecific(s),
    mSeverity(sv)
{}

//-Instance Functions-------------------------------------------------------------
//Public:
bool CoreError::isValid() const { return mType != NoError; }
QString CoreError::specific() const { return mSpecific; }
CoreError::Type CoreError::type() const { return mType; }

//Private:
Qx::Severity CoreError::deriveSeverity() const { return mSeverity; }
quint32 CoreError::deriveValue() const { return mType; }
QString CoreError::derivePrimary() const { return ERR_STRINGS.value(mType); }
QString CoreError::deriveSecondary() const { return mSpecific; }

//===============================================================================================================
// CORE
//===============================================================================================================

//-Constructor-------------------------------------------------------------
Core::Core(QCoreApplication* app) :
    QObject(app),
    mLogger(QFileInfo(QCoreApplication::applicationFilePath()).baseName() + '.' + LOG_FILE_EXT, app),
    mLogErrorOccurred(false),
    mArguments(app->arguments()),
    mRefElectionCfg(std::nullopt),
    mCalcOptions(Star::Calculator::NoOptions),
    mMinimal(false)
{
    // Logger tweaks
    mLogger.setMaximumEntries(50);
}

//-Instance Functions-------------------------------------------------------------
//Private:
void Core::handleLogError(const Qx::IoOpReport& error)
{
    // Note error
    mLogErrorOccurred = true;

    // Post error
    static CoreError errorMsg(CoreError::LogError, error.outcomeInfo(), Qx::Warning);
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

void Core::logElectionData(const ReferenceElectionConfig& data)
{
    logEvent(NAME, LOG_EVENT_ELECTION_DATA_PROVIDED.arg(data.bbPath, data.ccPath));
}

//Public:
Qx::Error Core::initialize()
{
    // Open log
    Qx::IoOpReport logOpen = mLogger.openLog();
    if(logOpen.isFailure())
        postError(NAME, Qx::Error(logOpen).setSeverity(Qx::Warning));

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
        CoreError err(CoreError::InvalidArgs, clParser.errorText());
        postError(NAME, err);
        showHelp();
        return err;
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
        // Setup election data container
        mRefElectionCfg = ReferenceElectionConfig{
            .ccPath = clParser.value(CL_OPTION_CONFIG),
            .bbPath = clParser.value(CL_OPTION_BOX),
        };

        logElectionData(mRefElectionCfg.value());

        // Handle calculator options
        QStringList selectedOpts;
        if(clParser.isSet(CL_OPTION_CALC_OPTIONS))
        {
            QStringList optStrs = clParser.value(CL_OPTION_CALC_OPTIONS).split(',', Qt::SkipEmptyParts);
            for(const QString& optStr : optStrs)
            {
                auto potentialOpt = magic_enum::enum_cast<Star::Calculator::Option>(optStr.toStdString());
                if(potentialOpt.has_value())
                {
                    selectedOpts.append(optStr);
                    mCalcOptions.setFlag(potentialOpt.value());
                }
                else
                {
                    CoreError err(CoreError::InvalidCalcOption, optStr);
                    postError(NAME, err);
                    return err;
                }
            }

        }

        QString optStr = !selectedOpts.isEmpty() ? selectedOpts.join(',') : ENUM_NAME(Star::Calculator::NoOptions);
        logEvent(NAME, LOG_EVENT_SELECTED_CALCULATOR_OPTIONS.arg(optStr));

        // Handle minimal option
        if(clParser.isSet(CL_OPTION_MINIMAL))
        {
            mMinimal = true;
            logEvent(NAME, LOG_EVENT_MINIMAL_MODE);
        }
    }
    else
    {
        CoreError err(CoreError::InvalidArgs, ERR_MISSING_REF_PATHS);
        logError(NAME, err);
        return err;
    }

    // Return success
    return CoreError();
}

bool Core::hasActionableArguments() const { return mRefElectionCfg.has_value(); }

ReferenceElectionConfig Core::referenceElectionConfig() const
{
    return mRefElectionCfg.has_value() ? mRefElectionCfg.value() : ReferenceElectionConfig();
}

Star::Calculator::Options Core::calculatorOptions() const { return mCalcOptions; }

bool Core::isMinimalPresentation() const { return mMinimal; }

//-Signals & Slots------------------------------------------------------------------------------------------------------------
//Public slots:
void Core::logError(const QString& src, const Qx::Error& error)
{
    if(!mLogErrorOccurred)
    {
        Qx::IoOpReport lr = mLogger.recordErrorEvent(src, error);
        if(lr.isFailure())
            handleLogError(lr);
    }
}

void Core::logEvent(const QString& src, const QString& event)
{
    if(!mLogErrorOccurred)
    {
        Qx::IoOpReport lr = mLogger.recordGeneralEvent(src, event);
        if(lr.isFailure())
            handleLogError(lr);
    }
}

void Core::logCalculatorDetail(const QString& detail) { logEvent("Calculator", detail); }

ErrorCode Core::logFinish(const Qx::Error& errorState)
{
    ErrorCode code = errorState.typeCode();

    if(!mLogErrorOccurred)
    {
        Qx::IoOpReport lr = mLogger.finish(code);
        if(lr.isFailure())
            handleLogError(lr);
    }

    return code;
}

void Core::postError(const QString& src, const Qx::Error& error)
{
    Qx::cout << error << Qt::endl;
    logError(src, error);
}

void Core::postMessage(const QString& msg)
{
    Qx::cout << msg << Qt::endl;
}
