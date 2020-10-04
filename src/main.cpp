#include <QApplication>
#include <dxflib/dl_dxf.h>

#include "fileio/dxf.h"
#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    //TODO test
    DL_Dxf *dxf = new DL_Dxf();
    auto *obj = new Object;
    DXFFilter filter(obj);
    bool success = dxf->in("../staging/no-contact.dxf", &filter);
    delete dxf;
    delete obj;
    //ODOT test
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
