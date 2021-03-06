#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "global.h"
#include "model/project.h"
#include "fileio/xml.h"
#include "fileio/pdf.h"
#include "ui/objects/gline.h"
#include "ui/objects/gcompositeobject.h"
#include "ui/operations.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QProcess> // For QtAssistant help

// For changing action icon color based on theme.
#include <QGraphicsColorizeEffect>
#include <QGraphicsItem>
#include <QSvgRenderer>
#include <QPainter>

// TODO rm
#include <QSvgGenerator>
#include <iostream>

MainWindow* MainWindow::instance{};

// CONSTRUCTORS

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    // Singleton instance
    instance = this;

    // Create widgets and menus...
    ui->setupUi(this);
    ui->menuView->addMenu(getPopupMenu());
    setupActions(); // Menu actions
    setupIcons(); // Action icons

    // Setup initial contents of widgets
    ui->tabView->clear();
    setVimStatus("");
    ui->statusbar->hide();

    // Override tab close requests
    connect(ui->tabView, &QTabWidget::tabCloseRequested,
            this, &MainWindow::onTabCloseRequested);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete popupMenu;
}

MainWindow *MainWindow::getInstance()
{
    return instance;
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

// SETTERS

void MainWindow::setTabId(int id)
{
    ui->tabView->setCurrentIndex(id);
}

void MainWindow::setVimStatus(const QString &status)
{
    ui->vimKeyStatus->setText(status);
}

QMenu *MainWindow::createPopupMenu()
{
    QMenu *menu = QMainWindow::createPopupMenu();
    if (menu == nullptr) menu = new QMenu;

    // Setup menu
    menu->setParent(this);
    menu->setToolTipsVisible(true);
    menu->setTitle("Widgets");

    // Populate menu
    menu->addSeparator();
    QAction *hideTabs = menu->addAction("Show tabs");
    QAction *menubar = menu->addAction("Show menu bar permanently");

    // Actions initialization
    hideTabs->setCheckable(true);
    hideTabs->setChecked(ui->tabView->tabBar()->isVisible());
    menubar->setToolTip("Do not display sheets as tabs");
    menubar->setCheckable(true);
    menubar->setChecked(menubarShownPermanently);
    menubar->setToolTip("Show/hide the menu bar "
                        "(hold Alt to temporarily show it)");
    // Connections
    connect(hideTabs, &QAction::triggered, menu, [this, hideTabs]() {
        ui->tabView->tabBar()->setVisible(hideTabs->isChecked());
        hideTabs->setChecked(hideTabs->isChecked());
    });
    connect(menubar, &QAction::triggered, menu, [this, menubar]() {
        showMenubarPermanently(menubar->isChecked());
        menubar->setChecked(menubarShownPermanently);
    });

    return menu;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // If the menu bar is shown only temporarily:
    // 1. If the user holds Alt, show the menu bar while he/she is holding it
    // 2. If the user presses a mouse button in any widget, hide the menu bar
    // TODO this is not complete, but it's not a priority to fix this. Problems:
    // 1. Menu actions don't work when the menubar is hidden
    // 2. When a menu is active, hiding the menubar doesn't hide the menu popup
    // 3. This implementation is embarrassingly inelegant either way, please help.
    if (!menubarShownPermanently)
    {
        if (obj != ui->menubar
                && event->type() == QEvent::MouseButtonPress)
            ui->menubar->hide();
        else if (event->type() == QEvent::KeyPress
                && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Alt)
            ui->menubar->show();
        else if (event->type() == QEvent::KeyRelease
                && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Alt)
            ui->menubar->hide();
    }
    return QMainWindow::eventFilter(obj, event);
}

// EVENTS

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    Vim::registerKeyPress(event, [this](const Vim::Action &action) {
        processVimAction(action);
        return true; // Top-level parent should return true
    });
    return QMainWindow::keyPressEvent(event);
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
                   QStringList{"-collectionFile", QT_HELP});
}

void MainWindow::takeScreenshot()
{
    QString filename = QFileDialog::getSaveFileName(
                this, "Open project file...",
                getSetting("screenshot_path", QDir::homePath() + "/screenshot.png").toString(),
                "Images (*.jpg *.png *.bmp *.svg);;All Files (*)"
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

void MainWindow::showMenubarPermanently(bool show)
{
    menubarShownPermanently = show;
    ui->menubar->setVisible(show);
    if (!show)
        // TODO add
        qApp->installEventFilter(this);
    else
        qApp->removeEventFilter(this);
}

void MainWindow::onTabCloseRequested(int index)
{
    auto *tab = getTab(index);
    ui->tabView->removeTab(index);
    delete tab;
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

// HELPERS

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

// Local helper function
QIcon svgColorChange(const QString &filename)
{
    // Open the svg file containing the icon
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
        return QIcon(filename);

    // Tweak the color to suit the application theme
    QString color = qApp->palette().color(QPalette::Text).name();
    QString modified =
            QString(file.readAll())
            .replace("\"#000000\"", "\"" + color + "\"")
            .replace("\"#000\"", "\"" + color + "\"");

    // Render the SVG and return an icon
    QSvgRenderer rr(modified.toUtf8());
    QImage image(12, 12, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    QPainter painter(&image);
    painter.setOpacity(0.6);
    rr.render(&painter);
    return QIcon(QPixmap::fromImage(image));
}

void MainWindow::setupIcons()
{
// TODO use this one #define PREFIX ICON_DIR "/actions/schim-"
#define PREFIX ICON_DIR "/actions/"
    ui->actionInsertRect->setIcon(svgColorChange(PREFIX "rect.svg"));
    ui->actionInsertLine->setIcon(svgColorChange(PREFIX "line.svg"));
#undef PREFIX
}

void MainWindow::populateWithProject()
{
    ui->tabView->clear();
    for (Sheet *sheet : *activeProject) //TODO changing for to foreach makes the program crash
        ui->tabView->addTab(new SheetView(sheet, ui->tabView), sheet->getTitle());
    // The tooltip is only useful before the user ever opens a sheet.
    ui->centralwidget->setToolTip("");
}

QMenu *MainWindow::getPopupMenu()
{
    if (popupMenu == nullptr)
        popupMenu = createPopupMenu();
    return popupMenu;
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
