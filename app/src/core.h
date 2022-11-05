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

// Base Includes
#include "star/calculator.h"

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
    static inline const QString LOG_ERR_INVALID_ARGS = QStringLiteral("Invalid arguments provided.");
    static inline const QString LOG_ERR_INVALID_EXTRA = QStringLiteral("Invalid extra tiebreak method.");

    // Logging - Messages
    static inline const QString LOG_EVENT_INIT = QStringLiteral("Initializing " PROJECT_SHORT_NAME "...");
    static inline const QString LOG_EVENT_G_HELP_SHOWN = QStringLiteral("Displayed general help information");
    static inline const QString LOG_EVENT_VER_SHOWN = QStringLiteral("Displayed version information");

    static inline const QString LOG_EVENT_ELECTION_DATA_PROVIDED = QStringLiteral(R"(Election data provided: { .bbPath = "%1", .ccPath = "%2", .extraTiebreak = %3 })");
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

    static inline const QString CL_OPT_EXTRA_S_NAME = QStringLiteral("e");
    static inline const QString CL_OPT_EXTRA_L_NAME = QStringLiteral("extra");
    static inline const QString CL_OPT_EXTRA_DESC_TEMPLATE = QStringLiteral("Performs an extra tiebreaker, if necessary. One of {%1}");
    static inline const QString CL_OPT_EXTRA_DESC = [](){
        constexpr auto choices = magic_enum::enum_names<Star::Calculator::ExtendedTiebreakMethod>();
        QStringList qChoices;
        for(const std::string_view& sv : choices)
            qChoices << QString(sv.data());
        return CL_OPT_EXTRA_DESC_TEMPLATE.arg(qChoices.join(", "));
    }();

    static inline const QString CL_OPT_MINIMAL_S_NAME = QStringLiteral("m");
    static inline const QString CL_OPT_MINIMAL_L_NAME = QStringLiteral("minimal");
    static inline const QString CL_OPT_MINIMAL_DESC = QStringLiteral("Only presents the results summary.");

    static inline const QString CL_OPT_SPECULATIVE_S_NAME = QStringLiteral("s");
    static inline const QString CL_OPT_SPECULATIVE_L_NAME = QStringLiteral("speculative");
    static inline const QString CL_OPT_SPECULATIVE_DESC = QStringLiteral("Logs the potential result of all extended tiebreak methods, regardless of if they're selected.");

    // Global command line options
    static inline const QCommandLineOption CL_OPTION_HELP{{CL_OPT_HELP_S_NAME, CL_OPT_HELP_L_NAME, CL_OPT_HELP_E_NAME}, CL_OPT_HELP_DESC}; // Boolean option
    static inline const QCommandLineOption CL_OPTION_VERSION{{CL_OPT_VERSION_S_NAME, CL_OPT_VERSION_L_NAME}, CL_OPT_VERSION_DESC}; // Boolean option
    static inline const QCommandLineOption CL_OPTION_CONFIG{{CL_OPT_CONFIG_S_NAME, CL_OPT_CONFIG_L_NAME}, CL_OPT_CONFIG_DESC, "config"}; // Takes value
    static inline const QCommandLineOption CL_OPTION_BOX{{CL_OPT_BOX_S_NAME, CL_OPT_BOX_L_NAME}, CL_OPT_BOX_DESC, "box"}; // Takes value
    static inline const QCommandLineOption CL_OPTION_EXTRA{{CL_OPT_EXTRA_S_NAME, CL_OPT_EXTRA_L_NAME}, CL_OPT_EXTRA_DESC, "extra"}; // Takes value
    static inline const QCommandLineOption CL_OPTION_MINIMAL{{CL_OPT_MINIMAL_S_NAME, CL_OPT_MINIMAL_L_NAME}, CL_OPT_MINIMAL_DESC}; // Boolean option
    static inline const QCommandLineOption CL_OPTION_SPECULATIVE{{CL_OPT_SPECULATIVE_S_NAME, CL_OPT_SPECULATIVE_L_NAME}, CL_OPT_SPECULATIVE_DESC}; // Boolean option

    static inline const QList<const QCommandLineOption*> CL_OPTIONS_ALL{&CL_OPTION_HELP, &CL_OPTION_VERSION, &CL_OPTION_CONFIG, &CL_OPTION_BOX, &CL_OPTION_EXTRA,
                                                                        &CL_OPTION_MINIMAL, &CL_OPTION_SPECULATIVE};

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
    bool hasActionableArguments();
    ReferenceElectionConfig referenceElectionConfig();
    bool isMinimalPresentation();

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
