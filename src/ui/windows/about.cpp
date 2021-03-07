#include "about.h"
#include "ui_about.h"

#include "symbols.h"

#include <QFile>
#include <QFileInfo>

#include <iostream>

About::About(QWidget *parent)
    : QDialog(parent), ui(new Ui::About)
{
    ui->setupUi(this);
    // Basic app info
    ui->logo->setPixmap(QPixmap(ICON_DIR "/apps/schim.svg"));
    ui->version->setText(VERSION);
    QColor color = qApp->palette().color(QPalette::Background);
    ui->github->setText(
                "<a href=\"https://github.com/schim-app\">"
                "<img src=\"qrc://img/" +
                QString(color.lightness() > 127 ? "github-dark" : "github-light") +
                ".svg\">"
                "</a>"
    );
    // TODO remove barely noticable frame that appears when you click the
    // link on the image

    // Show license for installed version of the program
    ui->license->setText("<a href=\"https://github.com/schim-app/schim/blob/v"
                        VERSION "/LICENSE\">License</a>");

    // Attributions text browser (HTML mode, don't let the extension confuse you)
    QFile file(DOC_DIR "/ATTRIBUTION.md");
    file.open(QIODevice::ReadOnly);
    ui->attribution->setHtml(file.readAll().replace("./", "qrc://"));
    // Appearance
    ui->attribution->setStyleSheet("background: transparent;");
    // TODO make the headings smaller
    // (attribution->document()->setDefaultStyleSheet doesn't work
}

About::~About()
{
    delete ui;
}
