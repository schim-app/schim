#include "cli_export.h"

#include "fileio/pdf.h"
#include "fileio/xml.h"

#include <QApplication>
#include <QFileInfo>
#include <memory>

namespace // Local 'private' functions
{
    void showHelp(const QString &text)
    {
        std::cout << text.toStdString()
                  << "\nFor more details run 'man schim-export'."
                  << std::endl;
    }

    void setupParser(QCommandLineParser &parser)
    {
        // Name that appears in an error message
        qApp->setApplicationName("schim export");

        // OPTIONS
        parser.addOption({{"h", "help"}, "Prints this help message."});
        parser.addOption({{"f", "format"}, "The output file format: pdf, svg (soon). Default: auto.", "FORMAT", ""});
        parser.addOption({{"o", "output"}, "Output file. Uses stdout if unspecified.", "FILE"});
        // POSITIONAL ARGS
        parser.addPositionalArgument("input", "The input file. Can also be '-' for stdin.");
    }

    int doExport(QString format, const QString &input, const QString &output)
    {
        if (format == "")
        { // Output format is auto-detected based on file extension
            QFileInfo finfo(output);
            QString ext = finfo.suffix(); // File extension
            if (ext.isEmpty()) // Output file has no extension
            {
                std::cerr << "schim export: Unable to detect format based on output file extension.\n"
                          << "Specify the output format with --format=<FORMAT> instead.\n";
                return 1; // TODO return what?
            }
            else
                format = ext;
        }
        if (format == "pdf")
        {
            Project *project = nullptr;
            // TODO add input formats other than XML
            try
            {
                Project *project = xmlParseProject(input);
                pdfWriteProject(project, output);
                delete project;
            }
            catch (std::exception &e)
            {
                delete project;
                std::cerr << "schim: " << e.what() << ".\n";
                return ErrCode::FileError;
            }
        }
        else if (format == "svg")
        {
            // TODO
            return 0;
        }
        else
        {
            std::cerr << "schim export: Unknown output format '" <<
                         format.toStdString() << "'.\n";
        }
        return 0;
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

int cli_export(QStringList args)
{
    QCommandLineParser parser;
    setupParser(parser);
    parser.process(args);

    // Help option
    if (parser.isSet("help"))
    {
        showHelp(parser.helpText());
        return 0;
    }

    convertToGuiApp();

    // Positional arguments
    auto posArgs = parser.positionalArguments();
    // Missing source file -- exit with error
    if (posArgs.size() != 1)
    {
        missing_positional("input");
        return ErrCode::MissingPosArg;
    }

    // Argument that contains the input file
    QString input = posArgs[0];

#ifndef Q_OS_UNIX // TODO Try to support Windows too, but 'tis not a priority.
    if (input == "-" || !parser.isSet("output"))
    {
        std::cerr << "schim: Exporting from/to stdin/stdout is not supported on your operating system.\n";
        return 1; // TODO return what?
    }
#endif

    if (input == "-")
        input = "/dev/stdin";

    QString output = "/dev/stdout";
    if (parser.isSet("output"))
        output = parser.value("output");

    return doExport(parser.value("format"), input, output);
}
