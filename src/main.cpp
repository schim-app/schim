#include <QApplication>
#include <list>

#include <model/component.h>
#include <model/line.h>

#include "global.h"
#include "ui/mainwindow.h"
#include "cli/cli_common.h"
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

int main(int argc, char *argv[])
{
    // Convert arguments array to QStringList
    QStringList args = argsToList(argc, argv);
    // Parse CLI arguments
    auto &parser = setupParser();
    parser.parse(args);

    // Show help and/or version and quit
    if (parser.isSet("help") || parser.isSet("version"))
    {
        // TODO using const_cast makes me a bit nervous, but seems
        // to be the easiest way to change the help message.
        argv[0] = const_cast<char*>("schim");
        QCoreApplication app(argc, argv); // This is necessary
        parser.process(args); // Print out unknown options and quit
        if (parser.isSet("version"))
            std::cout << "schim version " VERSION "\n";
        if (parser.isSet("help"))
            showHelp(parser.helpText()); // Show a tweaked version of help
        return 0;
    }

    auto posArgs = parser.positionalArguments();
    if (posArgs.empty() || posArgs[0] == "editor")
    {
        // TODO perhaps enable arguments for this command
        QApplication app(argc, argv);
        parser.process(args);
        (new MainWindow)->show();
        if (posArgs.size() >= 2)
            MainWindow::getInstance()->openProjectsFromFiles(
                        QStringList(posArgs.begin()+1, posArgs.end()), 0);
        return app.exec();
    }
    else
    {
        // Process any arguments before the command
        QCoreApplication *app = new QCoreApplication(argc, argv);
        parser.process(args);
        // This name is displayed in 'usage' information
        argv[0] = const_cast<char*>(("schim " + posArgs[0].toStdString()).c_str());
        delete app;
        app = new QCoreApplication(argc, argv);
        parser.process(args); // Print out unknown option error
        if (posArgs[0] == "export")
        {
            delete app; // Export will instantiate its own QApplication
            return cli_export(posArgs);
        }
        else
        {
            delete app;
            unknown_command(posArgs[0]);
            return ErrCode::UnrecognizedCommand;
        }
    }

    return 0;
}
