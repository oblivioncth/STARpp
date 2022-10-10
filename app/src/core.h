#ifndef CORE_H
#define CORE_H

// Qt Includes
#include <QObject>
#include <QCommandLineParser>

// Qx Includes
#include <qx/io/qx-applicationlogger.h>
#include <qx/core/qx-iostream.h>

// Project Includes
#include "errorcode.h"
#include "referenceelectionconfig.h"
#include "project_vars.h"

class Core : public QObject
{
    Q_OBJECT;
//-Class Variables------------------------------------------------------------------------------------------------------
private:
    // Error Messages
    static inline const QString ERR_LOG_ERROR = QStringLiteral("Error writing to log");
    static inline const QString ERR_MISSING_REF_PATHS = QStringLiteral("Paths for both a category config and ballot box must be provided in order to calculate an election winner");

    // Logging - Primary
    static inline const QString LOG_FILE_EXT = QStringLiteral("log");

    // Logging - Errors
    static inline const QString LOG_ERR_INVALID_ARGS = "Invalid arguments provided";

    // Logging - Messages
    static inline const QString LOG_EVENT_INIT = QStringLiteral("Initializing " PROJECT_SHORT_NAME "...");
    static inline const QString LOG_EVENT_G_HELP_SHOWN = "Displayed general help information";
    static inline const QString LOG_EVENT_VER_SHOWN = "Displayed version information";

    // Global command line option strings
    static inline const QString CL_OPT_HELP_S_NAME = QStringLiteral("h");
    static inline const QString CL_OPT_HELP_L_NAME = QStringLiteral("help");
    static inline const QString CL_OPT_HELP_E_NAME = QStringLiteral("?");
    static inline const QString CL_OPT_HELP_DESC = QStringLiteral("Prints this help message.");

    static inline const QString CL_OPT_VERSION_S_NAME = QStringLiteral("v");
    static inline const QString CL_OPT_VERSION_L_NAME = QStringLiteral("version");
    static inline const QString CL_OPT_VERSION_DESC = QStringLiteral("Prints the current version of this tool.");

    static inline const QString CL_OPT_CONFIG_S_NAME = QStringLiteral("c");
    static inline const QString CL_OPT_CONFIG_L_NAME = QStringLiteral("config");
    static inline const QString CL_OPT_CONFIG_DESC = QStringLiteral("Specifies the path to the category config INI file.");

    static inline const QString CL_OPT_BOX_S_NAME = QStringLiteral("b");
    static inline const QString CL_OPT_BOX_L_NAME = QStringLiteral("box");
    static inline const QString CL_OPT_BOX_DESC = QStringLiteral("Specifies the path to the ballot box CSV file.");

    static inline const QString CL_OPT_TOP_S_NAME = QStringLiteral("t");
    static inline const QString CL_OPT_TOP_L_NAME = QStringLiteral("top");
    static inline const QString CL_OPT_TOP_DESC = QStringLiteral("Performs a final tiebreaker, if necessary, by checking which candidate has the most 5 point scores.");

    // Global command line options
    static inline const QCommandLineOption CL_OPTION_HELP{{CL_OPT_HELP_S_NAME, CL_OPT_HELP_L_NAME, CL_OPT_HELP_E_NAME}, CL_OPT_HELP_DESC}; // Boolean option
    static inline const QCommandLineOption CL_OPTION_VERSION{{CL_OPT_VERSION_S_NAME, CL_OPT_VERSION_L_NAME}, CL_OPT_VERSION_DESC}; // Boolean option
    static inline const QCommandLineOption CL_OPTION_CONFIG{{CL_OPT_CONFIG_S_NAME, CL_OPT_CONFIG_L_NAME}, CL_OPT_CONFIG_DESC, "config"}; // Takes value
    static inline const QCommandLineOption CL_OPTION_BOX{{CL_OPT_BOX_S_NAME, CL_OPT_BOX_L_NAME}, CL_OPT_BOX_DESC, "box"}; // Takes value
    static inline const QCommandLineOption CL_OPTION_TOP{{CL_OPT_TOP_S_NAME, CL_OPT_TOP_L_NAME}, CL_OPT_TOP_DESC}; // Boolean option

    static inline const QList<const QCommandLineOption*> CL_OPTIONS_ALL{&CL_OPTION_HELP, &CL_OPTION_VERSION, &CL_OPTION_CONFIG, &CL_OPTION_BOX, &CL_OPTION_TOP};

    // Help template
    static inline const QString HELP_TEMPL = "<u>Usage:</u><br>"
                                             PROJECT_SHORT_NAME "&lt;options&gt;<br>"
                                             "<br>"
                                             "<u>Options:</u>%1<br>"
                                             "<br>";
    static inline const QString HELP_OPT_TEMPL = "<br><b>%1:</b> &nbsp;%2";

    // Command line messages
    static inline const QString CL_VERSION_MESSAGE = PROJECT_APP_NAME " " PROJECT_VERSION_STR ", reference application for the STAR voting method calculator";

    // Meta
    static inline const QString NAME = "Core";

//-Instance Variables------------------------------------------------------------------------------------------------------
private:
    // Log
    Qx::ApplicationLogger mLogger;
    bool mLogErrorOccurred;

    // Processing
    QStringList mArguments;
    std::optional<ReferenceElectionConfig> mRefElectionCfg;

//-Constructor----------------------------------------------------------------------------------------------------------
public:
    Core(QCoreApplication* app);

//-Instance Functions------------------------------------------------------------------------------------------------------
private:
    void handleLogError(Qx::IoOpReport error);
    void showHelp();
    void showVersion();

public:
    ErrorCode initialize();
    bool hasActionableArguments();
    ReferenceElectionConfig referenceElectionConfig();

//-Signals & Slots------------------------------------------------------------------------------------------------------------
public slots:
    void logError(QString src, Qx::GenericError error);
    void logEvent(QString src, QString event);
    void logCalculatorDetail(QString detail);
    ErrorCode logFinish(ErrorCode exitCode);

    void postError(QString src, Qx::GenericError error);
    void postMessage(QString msg);
};

#endif // CORE_H
