#ifndef CORE_H
#define CORE_H

// Qt Includes
#include <QObject>
#include <QCommandLineParser>

// Qx Includes
#include <qx/io/qx-applicationlogger.h>
#include <qx/core/qx-iostream.h>
#include <qx/core/qx-string.h>

// magic_enum Includes
#include <magic_enum.hpp>

// Project Includes
#include "errorcode.h"
#include "star/calculator.h"
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
    static inline const QString LOG_ERR_INVALID_ARGS = QStringLiteral("Invalid arguments provided.");

    // Logging - Messages
    static inline const QString LOG_EVENT_INIT = QStringLiteral("Initializing " PROJECT_SHORT_NAME "...");
    static inline const QString LOG_EVENT_G_HELP_SHOWN = QStringLiteral("Displayed general help information");
    static inline const QString LOG_EVENT_VER_SHOWN = QStringLiteral("Displayed version information");

    static inline const QString LOG_EVENT_ELECTION_DATA_PROVIDED = QStringLiteral(R"(Election data provided: { .bbPath = "%1", .ccPath = "%2" })");
    static inline const QString LOG_EVENT_SELECTED_CALCULATOR_OPTIONS = QStringLiteral("Selected calculator options: %1");
    static inline const QString LOG_EVENT_MINIMAL_MODE = QStringLiteral("Minimal presentation mode enabled.");

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

    static inline const QString CL_OPT_TRUE_TIES_S_NAME = QStringLiteral("t");
    static inline const QString CL_OPT_TRUE_TIES_L_NAME = QStringLiteral("true-ties");
    static inline const QString CL_OPT_TRUE_TIES_DESC = QStringLiteral("Ends an election prematurely instead of using a random tiebreaker when an unresovable tie occurs.");

    static inline const QString CL_OPT_EXTRA_TIEBREAK_S_NAME = QStringLiteral("e");
    static inline const QString CL_OPT_EXTRA_TIEBREAK_L_NAME = QStringLiteral("extra-tiebreak");
    static inline const QString CL_OPT_EXTRA_TIEBREAK_DESC = QStringLiteral("Uses the Condorcet protocol tiebreaker during the scoring round before the random tiebreaker if necessary.");

    static inline const QString CL_OPT_MINIMAL_S_NAME = QStringLiteral("m");
    static inline const QString CL_OPT_MINIMAL_L_NAME = QStringLiteral("minimal");
    static inline const QString CL_OPT_MINIMAL_DESC = QStringLiteral("Only presents the results summary.");

    // Global command line options
    static inline const QCommandLineOption CL_OPTION_HELP{{CL_OPT_HELP_S_NAME, CL_OPT_HELP_L_NAME, CL_OPT_HELP_E_NAME}, CL_OPT_HELP_DESC}; // Boolean option
    static inline const QCommandLineOption CL_OPTION_VERSION{{CL_OPT_VERSION_S_NAME, CL_OPT_VERSION_L_NAME}, CL_OPT_VERSION_DESC}; // Boolean option
    static inline const QCommandLineOption CL_OPTION_CONFIG{{CL_OPT_CONFIG_S_NAME, CL_OPT_CONFIG_L_NAME}, CL_OPT_CONFIG_DESC, "config"}; // Takes value
    static inline const QCommandLineOption CL_OPTION_BOX{{CL_OPT_BOX_S_NAME, CL_OPT_BOX_L_NAME}, CL_OPT_BOX_DESC, "box"}; // Takes value
    static inline const QCommandLineOption CL_OPTION_TRUE_TIES{{CL_OPT_TRUE_TIES_S_NAME, CL_OPT_TRUE_TIES_L_NAME}, CL_OPT_TRUE_TIES_DESC}; // Boolean option
    static inline const QCommandLineOption CL_OPTION_EXTRA_TIEBREAK{{CL_OPT_EXTRA_TIEBREAK_S_NAME, CL_OPT_EXTRA_TIEBREAK_L_NAME}, CL_OPT_EXTRA_TIEBREAK_DESC}; // Boolean option
    static inline const QCommandLineOption CL_OPTION_MINIMAL{{CL_OPT_MINIMAL_S_NAME, CL_OPT_MINIMAL_L_NAME}, CL_OPT_MINIMAL_DESC}; // Boolean option

    static inline const QList<const QCommandLineOption*> CL_OPTIONS_ALL{&CL_OPTION_HELP, &CL_OPTION_VERSION, &CL_OPTION_CONFIG, &CL_OPTION_BOX,
                                                                        &CL_OPTION_MINIMAL, &CL_OPTION_TRUE_TIES, &CL_OPTION_EXTRA_TIEBREAK};

    // Help template
    static inline const QString HELP_TEMPL = "Usage:\n"
                                             "------\n"
                                             PROJECT_SHORT_NAME " <options>\n"
                                             "\n"
                                             "Options:\n"
                                             "--------\n"
                                             "%1"
                                             "\n";
    static inline const QString HELP_OPT_TEMPL = "\n%1: %2";

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
    Star::Calculator::Options mCalcOptions;

    bool mMinimal;

//-Constructor----------------------------------------------------------------------------------------------------------
public:
    Core(QCoreApplication* app);

//-Instance Functions------------------------------------------------------------------------------------------------------
private:
    void handleLogError(Qx::IoOpReport error);
    void showHelp();
    void showVersion();

    void logElectionData(const ReferenceElectionConfig data);

public:
    ErrorCode initialize();
    bool hasActionableArguments() const;
    ReferenceElectionConfig referenceElectionConfig() const;
    Star::Calculator::Options calculatorOptions() const;
    bool isMinimalPresentation() const;

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
