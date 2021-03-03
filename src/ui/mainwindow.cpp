#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "global.h"
#include "fileio/xml.h"
#include "fileio/pdf.h"
#include "ui/objects/gline.h"
#include "ui/objects/gcompositeobject.h"
#include "ui/operations.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QProcess> // For QtAssistant help

// TODO rm
#include <QSvgGenerator>
#include <QDebug>

MainWindow* MainWindow::instance{};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabView->clear();

    setVimStatus("");
    ui->statusbar->hide();

    setupActions();

    // Override tab close requests
    connect(ui->tabView, &QTabWidget::tabCloseRequested, this, &MainWindow::onTabCloseRequested);

    instance = this;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// GETTERS

QTabWidget *MainWindow::getTabView() const
{
    return ui->tabView;
}

int MainWindow::getTabId() const
{
    return ui->tabView->currentIndex();
}

SheetView *MainWindow::getTab(int index)
{
    if (index == -1)
        return (SheetView*) ui->tabView->currentWidget();
    else
        return (SheetView*) ui->tabView->widget(index);
}

SheetScene *MainWindow::scene()
{
    return getTab() ? getTab()->scene() : nullptr;
}

Sheet *MainWindow::getSheet(int index)
{
    return getTab(index)->scene()->getSheet();
}

QString MainWindow::getFileName() const
{
    return filename;
}

MainWindow *MainWindow::getInstance()
{
    return instance;
}

// SETTERS

void MainWindow::setTabId(int id)
{
    ui->tabView->setCurrentIndex(id);
}

void MainWindow::setVimStatus(const QString &status)
{
    ui->vimKeyStatus->setText(status);
}

// EVENTS

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    Vim::registerKeyPress(event, [this](const Vim::Action &action) {
        processVimAction(action);
        return true; // Top-level parent
    });
}

// ACTION PROCESSING

void MainWindow::nextTab(Vim::N n)
{
    setTabId(getTabId() + n);
}

void MainWindow::prevTab(Vim::N n)
{
    setTabId(getTabId() - n);
}

void MainWindow::appendSheet(Vim::N n)
{
    if (!activeProject)
        return;

    n = qMin(int(n), 50);
    vimdo(n) {
        Sheet *sheet = new Sheet;
        activeProject->addSheet(sheet);
        ui->tabView->addTab(new SheetView(sheet, ui->tabView), sheet->getTitle());
    }
}

void MainWindow::newSheetBefore(Vim::N n)
{
    if (!activeProject)
        return;

    n = qMin(int(n), 50);
    vimdo(n) {
        Sheet *sheet = new Sheet;
        activeProject->getSheets().insert(getTabId(), sheet);
        ui->tabView->insertTab(getTabId(), new SheetView(sheet, ui->tabView), "New sheet");
    }
}

void MainWindow::newSheetAfter(Vim::N n)
{
    if (!activeProject)
        return;

    n = qMin(int(n), 50);
    vimdo(n) {
        Sheet *sheet = new Sheet;
        activeProject->getSheets().insert(getTabId() + 1, sheet);
        ui->tabView->insertTab(getTabId() + 1, new SheetView(sheet, ui->tabView), "New sheet");
    }
}

void MainWindow::closeTab(Vim::N n)
{
    if (n.raw() == 0)
        n = getTabId() + 1;
    onTabCloseRequested(n - 1);
}

void MainWindow::openSheetSettings(Vim::N n)
{
    if (n.raw() == 0)
        n = getTabId() + 1;

    if (n > ui->tabView->count())
        return;
    SheetSettings settings(this, n - 1);
    settings.exec();
}

void MainWindow::newProject()
{
    delete activeProject;
    // Set the new project as active
    activeProject = new Project();
    // Create a new sheet and add it to the project
    Sheet *firstSheet = new Sheet;
    activeProject->addSheet(firstSheet);

    populateWithProject();
}

void MainWindow::openProject()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open project file...",
                                         getSetting("default_path", QDir::homePath()).toString());
    // TODO add filters later
    if (filename == "")
        return;

    try
    {
        openProjectFromFile(filename);
        if (getTab()) scene()->setFocus();
    }
    catch (std::exception &e)
    {
        QMessageBox::critical(this, "Error", "Unable to read file");
    }
}

void MainWindow::save()
{
    if (activeProject == nullptr) return;

    if (filename == "")
    {
        saveAs();
        return;
    }

    try
    {
        xmlWriteProject(activeProject, filename);
    }
    catch (...)
    {
        QMessageBox::critical(this, "Error", "Unable to write file");
    }
}

void MainWindow::saveAs()
{
    if (activeProject == nullptr) return;

    QString filename = QFileDialog::getSaveFileName(this, "Save project...",
                            getSetting("default_path", QDir::homePath()).toString());

    if (filename == "")
        return;

    try
    {
        xmlWriteProject(activeProject, filename);

        this->filename = filename;
        changeSetting("default_path", filename);

        setWindowTitle(QString("Schim - ") + activeProject->getTitle() + " (" + filename + ")");
    }
    catch (...)
    {
        QMessageBox::critical(this, "Error", "Unable to write file");
    }
}

void MainWindow::print()
{
    if (activeProject == nullptr) return;

    QString filename = QFileDialog::getSaveFileName(this, "Print to file...",
                            getSetting("print_path", QFileInfo(this->filename).absolutePath()).toString());

    if (filename == "") return; // No file chosen

    try
    {
        pdfWriteProject(activeProject, filename);
        changeSetting("print_path", filename);
    }
    catch (...)
    {
        QMessageBox::critical(this, "Error", "Unable to write file");
    }
}

void MainWindow::showHelp()
{
    // TODO make attribute of MainWindow, take care of destruction, etc.
    QProcess *process = new QProcess;
    process->start(QLatin1String("assistant"),
                   QStringList{"-collectionFile", qApp->applicationDirPath() + "/" QT_HELP});
}

void MainWindow::takeScreenshot()
{
    QString filename = QFileDialog::getSaveFileName(
                this, "Open project file...",
                getSetting("screenshot_path", QDir::homePath() + "/screenshot.png").toString(),
                "Images (*.jpg *.png *.bmp);;SVG Images (*.svg);;All Files (*)"
            );
    // TODO add filters later
    if (filename == "")
        return;

    QFileInfo info(filename);

    if (info.suffix() == "svg")
    {
        QSvgGenerator gen;
        gen.setFileName(filename);
        gen.setSize(size());
        gen.setViewBox(rect());
        //TODO How to check if the operation was successful??
        render(&gen);
    }
    else if (!grab().save(filename))
    {
        //TODO This scenario has not been tested
        QMessageBox::critical(this, "Error", "Unable to write file");
        return;
    }
    changeSetting("screenshot_path", filename);
}

void MainWindow::toggleDeveloperHints()
{
    ui->statusbar->setVisible(ui->statusbar->isHidden());
}

void MainWindow::onTabCloseRequested(int index)
{
    ui->tabView->removeTab(index);
    delete ui->tabView->widget(index);
}

bool MainWindow::processVimAction(const Vim::Action &action)
{
#define if_eq_do(name, act) if (action == name) { act(Vim::n()); }

         if_eq_do("tab-next",				nextTab)
    else if_eq_do("tab-prev",				prevTab)
    else if_eq_do("tab-close",				closeTab)
    else if_eq_do("new-sheet-before",		newSheetBefore)
    else if_eq_do("new-sheet-after",		newSheetAfter)
    else if_eq_do("sheet-append",			appendSheet)
    else if_eq_do("sheet-settings",			openSheetSettings)
    else return false;
    return true;
}

// MISCELLANEOUS

void MainWindow::setupActions()
{
    // File
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newProject);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openProject);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::print);
    // Edit
    connect(ui->actionUndoInSheet, &QAction::triggered, this,
            [this]() { if (getTab()) scene()->undo(); });
    connect(ui->actionRedoInSheet, &QAction::triggered,
            [this]() { if (getTab()) scene()->redo(); });
    // Sheet
    connect(ui->actionNewSheet, &QAction::triggered, this, &MainWindow::newSheetAfter);
    // Help
    connect(ui->actionHelp, &QAction::triggered, this, &MainWindow::showHelp);
    connect(ui->actionScreenshot, &QAction::triggered, this, &MainWindow::takeScreenshot);
    connect(ui->actionDeveloperHints, &QAction::triggered, this, &MainWindow::toggleDeveloperHints);
    // Toolbar
    connect(ui->actionInsertLine, &QAction::triggered,
            this, [this]() { if (getTab()) scene()->insertLine(); });
    connect(ui->actionInsertRect, &QAction::triggered,
            this, [this]() { if (getTab()) scene()->insertRect(); });
    connect(ui->actionInsertText, &QAction::triggered,
            this, [this]() { if (getTab()) scene()->insertText(); });
    // SheetView
    connect(ui->actionZoomIn, &QAction::triggered,
            this, [this]() { if (getTab()) getTab()->zoomIn(); });
    connect(ui->actionZoomOut, &QAction::triggered,
            this, [this]() { if (getTab()) getTab()->zoomOut(); });
    connect(ui->actionZoomReset, &QAction::triggered,
            this, [this]() { if (getTab()) getTab()->resetZoom(); });
}

void MainWindow::populateWithProject()
{
    ui->tabView->clear();
    for (Sheet *sheet : *activeProject) //TODO changing for to foreach makes the program crash
        ui->tabView->addTab(new SheetView(sheet, ui->tabView), sheet->getTitle());
    // The tooltip is only useful before the user ever opens a sheet.
    ui->centralwidget->setToolTip("");
}

void MainWindow::clearTabs()
{
    for (int i = 0; i < ui->tabView->count(); ++i)
        delete ui->tabView->widget(i);
    ui->tabView->clear();
}

void MainWindow::openProjectFromFile(const QString &filename)
{
    activeProject = xmlParseProject(filename);

    this->filename = filename;
    changeSetting("default_path", filename);

    setWindowTitle(QString("Schim - ") + activeProject->getTitle() + " (" + filename + ")");
    populateWithProject();
}
