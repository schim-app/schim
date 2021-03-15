#include "cli_editor.h"

#include "ui/mainwindow.h"

#include <QApplication>

namespace // Local 'private' functions
{
    void showHelp(const QString &text)
    {
        std::cout << text.toStdString()
                  << "\nFor more details run 'man schim-editor'."
                  << std::endl;
    }

    QCommandLineParser &setupParser()
    {
        // Name that appears in an error message
        qApp->setApplicationName("schim export");

        QCommandLineParser &parser = *new QCommandLineParser;
        // OPTIONS
        parser.addOption({{"h", "help"}, "Prints this help message."});
        // POSITIONAL ARGS
        parser.addPositionalArgument(
                    "input-files",
                    "The input files to open in the editor.");

        return parser;
    }
}

// Convert QCoreApplication to QApplication
#define convertAppToGui() \
        int argc = 1; char *argv[] = {const_cast<char*>("")}; \
        delete qApp; \
        new QApplication(argc, argv);

int cli_editor(const QStringList &args)
{
    QCommandLineParser &parser = setupParser();
    parser.process(args);

    if (parser.isSet("help"))
    {
        showHelp(parser.helpText());
        return 0;
    }

    // Required so we can use Qt's GUI module
    convertAppToGui();

    // Open the main window
    (new MainWindow)->show();

    // Open files that may have been specified
    auto posArgs = parser.positionalArguments();
    if (!posArgs.isEmpty())
        MainWindow::getInstance()->openProjectsFromFiles(
                QStringList(posArgs.begin(), posArgs.end()), 0);

    return qApp->exec();
}
