#include <QApplication>

#include "fileio/dxf.h"
#include "ui/mainwindow.h"
#include "global.h"

// TODO temporary
#include "model/project.h"
#include "model/line.h"
#include "fileio/xml.h"
#include <iostream>
#include <QXmlStreamReader>
#include <dxflib/dl_dxf.h>

// Temporary functions for testing

void testDxf()
{
    DL_Dxf *dxf = new DL_Dxf();
    auto *obj = new CompositeObject;
    DXFFilter filter(obj);
    bool success = dxf->in("../../staging/no-contact.dxf", &filter);
    delete dxf;
    delete obj;
}

void testXml()
{
}

int main(int argc, char *argv[])
{
    //TODO test
    //ODOT test
    QApplication a(argc, argv);
    mainWindow = new MainWindow;
    mainWindow->show();
    return a.exec();
}
