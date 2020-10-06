#include <QApplication>
#include <dxflib/dl_dxf.h>

#include "fileio/dxf.h"
#include "ui/mainwindow.h"
#include "global.h"

// TODO temporary
#include "model/project.h"
#include "fileio/xml.h"

// Temporary functions for testing

void testDxf()
{
    DL_Dxf *dxf = new DL_Dxf();
    auto *obj = new CompositeObject;
    DXFFilter filter(obj);
    bool success = dxf->in("../staging/no-contact.dxf", &filter);
    delete dxf;
    delete obj;
}

void testXml()
{
    Project *proj = xmlParseProject("../staging/sample.xml");
    int x = 4;
    delete proj;
}

int main(int argc, char *argv[])
{
    //TODO test
    testDxf();
    //ODOT test
    QApplication a(argc, argv);
    mainWindow = new MainWindow;
    mainWindow->show();
    return a.exec();
}
