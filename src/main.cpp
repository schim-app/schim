#include <QApplication>

#include "fileio/dxf.h"
#include "ui/mainwindow.h"
#include "global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    mainWindow = new MainWindow;
    mainWindow->show();
    return a.exec();
}
