#include "pdf.h"

#include "global.h"
#include "ui/sheetscene.h"

#include <QFile>
#include <QPainter>
#include <QPrinter>

void pdfWriteProject(const Project *project, const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        throw std::runtime_error(filename.toStdString() + ": unable to open file for writing");

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);
    printer.setMargins({});
    printer.setPageSize(QPrinter::A4);
    printer.setOrientation(QPrinter::Landscape);

    QPainter painter(&printer);
    for (int i = 0; i < project->size(); ++i)
    {
        if (i != 0)
        {
            printer.setPageSize(QPrinter::A4);
            printer.setOrientation(QPrinter::Landscape);
            printer.newPage();
        }
        SheetScene *scene = new SheetScene((*project)[i]);
        scene->render(&painter);
    }
}
