#include <QApplication>

#include "global.h"
#include "ui/mainwindow.h"

//TODO remove
#include "fileio/miscfiles.h"
#include "fileio/xml.h"
#include <QDir>
#include <iostream>


int main(int argc, char *argv[])
{

    QDir dir("../../symb");
    int lvl = 0;
    //iterate("../../symb", lvl);

    QApplication a(argc, argv);
    (new MainWindow)->show();
    return a.exec();
}
