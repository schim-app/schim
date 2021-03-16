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

    void setupParser(QCommandLineParser &parser)
    {
        // Name that appears in an error message
        qApp->setApplicationName("schim export");

        // OPTIONS
        parser.addOption({{"h", "help"}, "Prints this help message."});
        // POSITIONAL ARGS
        parser.addPositionalArgument(
                    "input-files",
                    "The input files to open in the editor.");
    }

    void convertToGuiApp()
    {
        // Create some dummy variables
        static int argc = 1;
        static std::unique_ptr<char[]> argv_0(new char[1]{});
        static std::unique_ptr<char*[]> argv(new char*[1]{argv_0.get()});
        // Required so we can use Qt's GUI module
        delete qApp;
        new QApplication(argc, argv.get());
    }

} // namespace

int cli_editor(const QStringList &args)
{
    QCommandLineParser parser;
    setupParser(parser);
    parser.process(args);

    if (parser.isSet("help"))
    {
        showHelp(parser.helpText());
        return 0;
    }

    convertToGuiApp();

    // Open the main window
    MainWindow *window = new MainWindow;
    window->show();

    // Open any files that may have been specified
    auto posArgs = parser.positionalArguments();
    if (!posArgs.isEmpty())
        MainWindow::getInstance()->openProjectsFromFiles(posArgs, 0);

    int code = qApp->exec();
    delete window;
    return code;
}
