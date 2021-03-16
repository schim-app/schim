#include "global.h"
#include "ui/mainwindow.h"
#include "cli/cli_common.h"
#include "cli/cli_editor.h"
#include "cli/cli_export.h"

#include <QApplication>
#include <memory>

void showHelp(const QString &text)
{
    std::cout << text.toStdString() << '\n'
              << "Available commands:\n"
              << "  editor\t Launch the graphical editor. [default]\n"
              << "  export\t Export schim projects into various formats.\n"
              << "  report\t Generate reports for schim projects.\n"
              << "\nFor command-specific help run 'schim <command> -h'."
              << "\nFor more details run 'man schim'."
              << std::endl;
}

inline QStringList argsToList(int argc, char *argv[])
{
    QStringList args;
    args.reserve(argc);
    for (int i = 0; i < argc; ++i)
        args.push_back(QString(argv[i]));
    return args;
}

/**
 * @brief Setup the parser for the command `schim` with general options.
 */
void setupParser(QCommandLineParser &parser)
{
    parser.setOptionsAfterPositionalArgumentsMode(
                QCommandLineParser::ParseAsPositionalArguments);

    // OPTIONS
    QCommandLineOption optHelp({"h", "help"}, "Prints this help message.");
    QCommandLineOption optVersion({"v", "version"}, "Displays version information.");

    parser.addOption(optHelp);
    parser.addOption(optVersion);

    parser.addPositionalArgument("command", "The schim command to run.");
}

/**
 * @brief Rename the application to `name`.
 * @param name The new name for the application.
 *
 * `QCommandLineParser` provides default help messages and usage information.
 * When it displays this information, it uses `argv[0]` that was passed to the
 * currently active `qApp` object as the name of the executable. In this
 * function, the contents of `argv[0]` are set to `name`.
 *
 */
void renameApp(const QString &name, char **argv)
{
    static std::unique_ptr<char[]> argv_0; // will be a smart pointer to argv[0]
    int len = name.length();

    // Note: any previous object owned by the unique_ptr will be deleted
    argv_0 = std::unique_ptr<char[]>(new char[len + 1]{});

    argv[0] = argv_0.get();
    // argv[0] shall contain `name`
    strncpy(argv[0], name.toStdString().c_str(), len);
}

/**
 * @brief Call a function that corresponds to the given command that is contained
 * in `args`.
 */
int delegateCommand(const QStringList &args)
{
    // The command was just `schim`
    if (args.empty())
        return cli_editor({""});
    else if (args[0] == "editor")
        return cli_editor(args);
    else if (args[0] == "export")
        return cli_export(args);
    else
    {
        unknown_command(args[0]);
        return ErrCode::UnrecognizedCommand;
    }
}

int main(int argc, char *argv[])
{
    // Convert arguments array to QStringList
    QStringList args = argsToList(argc, argv);
    // Parse CLI arguments
    QCommandLineParser parser;
    setupParser(parser);
    parser.parse(args);

    new QCoreApplication(argc, argv);

    // The command was `schim --help` or `schim --version`, or their shortened
    // versions, or any combination of those
    if (parser.isSet("help") || parser.isSet("version"))
    {
        renameApp("schim", argv);
        parser.process(args); // Potentially print out unknown options and quit
        if (parser.isSet("version"))
            std::cout << "schim version " VERSION "\n";
        if (parser.isSet("help"))
            showHelp(parser.helpText()); // Show a tweaked version of help
        return 0;
    }

    // Process any arguments before the command
    parser.process(args);

    auto posArgs = parser.positionalArguments();
    // Rename the app for proper display in auto-generated help message
    QString appName = posArgs.isEmpty() ? "schim" : "schim " + posArgs[0];
    renameApp(appName, argv);

    // Let the command be handled by its corresponding function
    delegateCommand(posArgs);

    delete qApp;
    return 0;
}
