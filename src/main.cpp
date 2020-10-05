#include <QApplication>
#include <dxflib/dl_dxf.h>

#include "fileio/dxf.h"
#include "ui/mainwindow.h"

// TODO temporary
#include "model/project.h"
#include "fileio/xml.h"

// Temporary functions for testing

void testDxf()
{
    DL_Dxf *dxf = new DL_Dxf();
    auto *obj = new Object;
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
    testXml();
    //ODOT test
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
