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
        throw std::runtime_error("Unable to open file '" + filename.toStdString() + " for writing");

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filename);
    printer.setPageMargins({});
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageOrientation(QPageLayout::Landscape);

    QPainter painter(&printer);
    for (int i = 0; i < project->getSheets().size(); ++i)
    {
        if (i != 0)
        {
            printer.setPageSize(QPageSize(QPageSize::A4));
            printer.setPageOrientation(QPageLayout::Landscape);
            printer.newPage();
        }
        SheetScene *scene = new SheetScene((project->getSheets())[i]);
        scene->render(&painter);
    }
}
