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

    // This should be called first
    ui->setupUi(this);
    restoreSettings();

    // Create remaining widgets and menus...
    ui->menuView->addMenu(getPopupMenu());
    setupActions(); // Menu actions
    setupIcons(); // Action icons

    // Setup initial contents of widgets
    ui->tabWidget->clear();
    setVimStatus("");
    ui->statusBar->hide();

    // Override tab close requests
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
            this, &MainWindow::onTabCloseRequested);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete popupMenu;
    delete globalDatabase;
}

MainWindow *MainWindow::getInstance()
{
    return instance;
}

// GETTERS

QTabWidget *MainWindow::getTabView() const
{
    return ui->tabWidget;
}

int MainWindow::getTabId() const
{
    return ui->tabWidget->currentIndex();
}

SheetView *MainWindow::getTab(int index)
{
    if (index == -1)
        return (SheetView*) ui->tabWidget->currentWidget();
    else
        return (SheetView*) ui->tabWidget->widget(index);
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
    ui->tabWidget->setCurrentIndex(id);
}

void MainWindow::setVimStatus(const QString &status)
{
    ui->vimKeyStatus->setText(status);
}

// OVERRIDEN

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
    QAction *showTabs = menu->addAction("Show tabs");
    QAction *menuBar = menu->addAction("Show menu bar permanently");

    // Actions initialization
    showTabs->setCheckable(true);
    showTabs->setChecked(ui->tabWidget->tabBar()->isVisible());
    menuBar->setToolTip("Do not display sheets as tabs");
    menuBar->setCheckable(true);
    menuBar->setChecked(menuBarShownPermanently);
    menuBar->setToolTip("Show/hide the menu bar "
                        "(hold Alt to temporarily show it)");
    // Connections
    connect(showTabs, &QAction::triggered, menu, [this, showTabs]() {
        ui->tabWidget->tabBar()->setVisible(showTabs->isChecked());
        showTabs->setChecked(showTabs->isChecked());
    });
    connect(menuBar, &QAction::triggered, menu, [this, menuBar]() {
        showMenubarPermanently(menuBar->isChecked());
        menuBar->setChecked(menuBarShownPermanently);
    });

    return menu;
}

// EVENTS

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    // If the menu bar is shown only temporarily:
    // 1. If the user holds Alt, show the menu bar while he/she is holding it
    // 2. If the user presses a mouse button in any widget, hide the menu bar
    // TODO this is not complete, but it's not a priority to fix this. Problems:
    // 1. Menu actions don't work when the menuBar is hidden
    // 2. When a menu is active, hiding the menuBar doesn't hide the menu popup
    // 3. This implementation is embarrassingly inelegant either way, please help.
    if (!menuBarShownPermanently)
    {
        if (obj != ui->menuBar
                && event->type() == QEvent::MouseButtonPress)
            ui->menuBar->hide();
        else if (event->type() == QEvent::KeyPress
                && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Alt)
            ui->menuBar->show();
        else if (event->type() == QEvent::KeyRelease
                && static_cast<QKeyEvent*>(event)->key() == Qt::Key_Alt)
            ui->menuBar->hide();
    }
    return QMainWindow::eventFilter(obj, event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    Vim::registerKeyPress(event, [this](const Vim::Action &action) {
        processVimAction(action);
        return true; // Top-level parent should return true
    });
    return QMainWindow::keyPressEvent(event);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    saveSettings();
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
        ui->tabWidget->addTab(new SheetView(sheet, ui->tabWidget), sheet->getTitle());
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
        ui->tabWidget->insertTab(getTabId(),
                new SheetView(sheet, ui->tabWidget), "New sheet");
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
        ui->tabWidget->insertTab(getTabId() + 1,
                new SheetView(sheet, ui->tabWidget), "New sheet");
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

    if (n > ui->tabWidget->count())
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
    ui->statusBar->setVisible(ui->statusBar->isHidden());
}

void MainWindow::showMenubarPermanently(bool show)
{
    menuBarShownPermanently = show;
    ui->menuBar->setVisible(show);
    if (!show)
        // TODO add
        qApp->installEventFilter(this);
    else
        qApp->removeEventFilter(this);
}

void MainWindow::restoreSettings()
{
    // Volatile settings
    restoreGeometry(getSetting("GUI/geometry", {}).toByteArray());
    restoreState(getSetting("GUI/windowState", {}).toByteArray());

    // Normal settings
    ui->toolBar->setVisible(getSetting("GUI/toolBar", true).toBool());
    showMenubarPermanently(getSetting("GUI/menuBar", true).toBool());
    ui->tabWidget->tabBar()->setVisible(getSetting("GUI/showTabs", true).toBool());

    // Vim-mode
    // TODO move this code to the place where vim mode is changed. Same in saveSettings
    Vim::enable(getSetting("UI/enableVimMode", false).toBool());
}

void MainWindow::saveSettings()
{
    // Volatile settings
    changeSetting("GUI/geometry", saveGeometry(), false);
    changeSetting("GUI/windowState", saveState(), true);

    // Normal settings
    changeSetting("GUI/toolBar", ui->toolBar->isVisible(), false);
    changeSetting("GUI/menuBar", menuBarShownPermanently, false);
    changeSetting("GUI/showTabs", ui->tabWidget->tabBar()->isVisible(), false);
    // Vim-mode
    changeSetting("UI/enableVimMode", Vim::enabled(), true);
}

void MainWindow::onTabCloseRequested(int index)
{
    auto *tab = getTab(index);
    ui->tabWidget->removeTab(index);
    delete tab;
}

bool MainWindow::processVimAction(const Vim::Action &action)
{
#define if_eq_do(name, act) if (action == name) { act(Vim::n()); }

         if_eq_do("tab-next",				nextTab)
    else if_eq_do("tab-prev",				prevTab)
    else if (action == "tab-first")			setTabId(0);
    else if (action == "tab-last")			setTabId(ui->tabWidget->count() - 1);
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
    connect(ui->actionExit, &QAction::triggered, this, [this](){ close(); });
    // Edit
    connect(ui->actionUndoInSheet, &QAction::triggered, this,
            [this]() { if (getTab()) scene()->undo(); });
    connect(ui->actionRedoInSheet, &QAction::triggered,
            [this]() { if (getTab()) scene()->redo(); });
    // Sheet
    connect(ui->actionNewSheet, &QAction::triggered,
            this, &MainWindow::newSheetAfter);
    // Help
    connect(ui->actionHelp, &QAction::triggered,
            this, &MainWindow::showHelp);
    connect(ui->actionScreenshot, &QAction::triggered,
            this, &MainWindow::takeScreenshot);
    connect(ui->actionDeveloperHints, &QAction::triggered,
            this, &MainWindow::toggleDeveloperHints);
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
    painter.setOpacity(0.5);
    rr.render(&painter);
    return QIcon(QPixmap::fromImage(image));
}

void MainWindow::setupIcons()
{
// TODO maybe use this one #define PREFIX ICON_DIR "/actions/schim-"
#define __ACTIONS ICON_DIR "/actions/"
    ui->actionInsertRect->setIcon(svgColorChange(__ACTIONS "rect.svg"));
    ui->actionInsertLine->setIcon(svgColorChange(__ACTIONS "line.svg"));
#undef PREFIX
    ui->actionAboutSchim->setIcon(QIcon(ICON_DIR "/apps/schim.svg"));
}

void MainWindow::populateWithProject()
{
    ui->tabWidget->clear();
    for (Sheet *sheet : *activeProject) //TODO changing for to foreach makes the program crash
        ui->tabWidget->addTab(new SheetView(sheet, ui->tabWidget), sheet->getTitle());
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
    for (int i = 0; i < ui->tabWidget->count(); ++i)
        delete ui->tabWidget->widget(i);
    ui->tabWidget->clear();
}

void MainWindow::openProjectFromFile(const QString &filename)
{
    activeProject = xmlParseProject(filename);

    this->filename = filename;
    changeSetting("default_path", filename);

    setWindowTitle(QString("Schim - ") + activeProject->getTitle()
            + " (" + filename + ")");
    populateWithProject();
}
