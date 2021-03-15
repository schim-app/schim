#include <QApplication>
#include <list>

#include <model/component.h>
#include <model/line.h>

#include "global.h"
#include "ui/mainwindow.h"
#include "cli/cli_common.h"
#include "cli/cli_editor.h"
#include "cli/cli_export.h"

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
inline QCommandLineParser &setupParser()
{
    QCommandLineParser &parser = *new QCommandLineParser;
    parser.setOptionsAfterPositionalArgumentsMode(
                QCommandLineParser::ParseAsPositionalArguments);

    // OPTIONS
    QCommandLineOption optHelp({"h", "help"}, "Prints this help message.");
    QCommandLineOption optVersion({"v", "version"}, "Displays version information.");

    parser.addOption(optHelp);
    parser.addOption(optVersion);

    parser.addPositionalArgument("command", "The schim command to run.");
    return parser;
}

/**
 * @brief Rename the application to `name`.
 * @param name The new name for the application.
 *
 * `QCommandLineParser` provides default help messages and usage information.
 * When it displays this information, it uses `argv[0]` that was passed to the
 * currently active `qApp` object as the name of the executable. In this
 * function, `argv[0]` is set to `name` and a new application object is
 * allocated, which is now configured with `name` as the executable name.
 *
 * @note The parameters `argc` and `argv` are used only so that the application
 * object can be constructed properly.
 */
void renameApp(const QString &name, int &argc, char **argv)
{
    int len = name.length();
    auto *str = const_cast<char*>(name.toStdString().c_str());
    argv[0] = new char[len + 1];
    strncpy(argv[0], str, len);
    new QCoreApplication(argc, argv);
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
    auto &parser = setupParser();
    parser.parse(args);

    // The command was `schim --help` or `schim --version`, or both, or their
    // short versions
    if (parser.isSet("help") || parser.isSet("version"))
    {
        renameApp("schim", argc, argv);
        parser.process(args); // Potentially print out unknown options and quit
        if (parser.isSet("version"))
            std::cout << "schim version " VERSION "\n";
        if (parser.isSet("help"))
            showHelp(parser.helpText()); // Show a tweaked version of help
        return 0;
    }

    auto posArgs = parser.positionalArguments();
    new QCoreApplication(argc, argv);
    // Process any arguments before the command
    parser.process(args);
    // Create a new app with the new application name
    delete qApp;
    QString appName = posArgs.isEmpty() ? "schim" : "schim " + posArgs[0];
    renameApp(appName, argc, argv);

    // Let the command be handled by its corresponding function
    delegateCommand(posArgs);
    delete qApp;

    return 0;
}
