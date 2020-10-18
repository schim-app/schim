#include <QApplication>

#include "fileio/dxf.h"
#include "global.h"
#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    (new MainWindow)->show();
    return a.exec();
}
