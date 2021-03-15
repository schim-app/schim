#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "global.h"
#include "model/project.h"
#include "fileio/xml.h"
#include "fileio/pdf.h"
#include "ui/objects/gline.h"
#include "ui/objects/gcompositeobject.h"
#include "ui/operations.h"
#include "ui/windows/about.h"
#include "ui/windows/projectsettings.h"
#include "ui/windows/preferences.h"
#include "ui/widgets/projectbrowser.h"
#include "ui/windows/sheetsettings.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>
#include <QProcess> // For QtAssistant help

// For changing action icon color based on theme.
#include <QGraphicsColorizeEffect>
#include <QGraphicsItem>
#include <QSvgRenderer>
#include <QPainter>
#include <QSvgGenerator>

#include <iostream>

/// Singleton instance
MainWindow *MainWindow::instance{};

// CONSTRUCTORS

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    // Singleton instance
    instance = this;

    // SETUP - should be called first
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
    ui->projectBrowser->setModel(projects = new ProjectManager);

    // Override tab close requests for proper cleanup
    connect(ui->tabWidget, &QTabWidget::tabCloseRequested,
            this, &MainWindow::onTabCloseRequested);

    setFocus();
}

MainWindow::~MainWindow()
{
    delete popupMenu;
    delete globalDatabase;
    delete projects;
    delete ui;
}

MainWindow *MainWindow::getInstance()
{
    return instance;
}

// GETTERS

QTabWidget *MainWindow::getTabWidget() const
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

bool MainWindow::isOpen(Sheet *sheet)
{
    for (int i = 0; i < ui->tabWidget->count(); ++i)
        if (getSheet(i) == sheet)
            return true;
    return false;
}

ProjectManager *MainWindow::getProjectManager()
{
    return projects;
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

void MainWindow::setActiveProject(Project *project)
{
    if (projects->getActiveProject() == project)
        return;

    closeSheetsForProject(projects->getActiveProject());
    projects->setActiveProject(project);
    if (project == nullptr) return;
    setWindowTitle(QString("Schim - ") + project->getName()
               + " (" + project->getFileName() + ")");
    ui->projectBrowser->setCurrentIndex(projects->getIndex(project));

    if (project->getFileName() != "")
        changeSetting("default_path", resolvePath(project->getFileName()));
}

// USER ACTIONS

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
    if (!projects->getActiveProject())
        return;

    n = qMin(int(n), 10);
    vimdo(n) {
        Sheet *sheet = new Sheet;
        projects->addSheet(sheet);
        ui->projectBrowser->setCurrentIndex(projects->getIndex(sheet));
        openSheet(sheet);
    }
}

void MainWindow::newSheetBefore(Vim::N n)
{
    if (!projects->getActiveProject())
        return;

    n = qMin(int(n), 10);
    vimdo(n) {
        Sheet *sheet = new Sheet;
        projects->addSheet(sheet, getTabId());
        ui->projectBrowser->setCurrentIndex(projects->getIndex(sheet));
        openSheet(sheet, getTabId());
    }
}

void MainWindow::newSheetAfter(Vim::N n)
{
    if (!projects->getActiveProject())
        return;

    n = qMin(int(n), 10);
    vimdo(n) {
        int tabId = getTabId() == -1 ? 0 : getTabId();
        Sheet *sheet = new Sheet;
        projects->addSheet(sheet, tabId + 1);
        ui->projectBrowser->setCurrentIndex(projects->getIndex(sheet));
        openSheet(sheet, tabId + 1);
    }
}

void MainWindow::closeTab(Vim::N n)
{
    if (n.raw() == 0)
        n = getTabId() + 1;
    onTabCloseRequested(n - 1);
}

void MainWindow::sheetSettings(Vim::N n)
{
    if (n.raw() == 0)
        n = getTabId() + 1;

    if (n > ui->tabWidget->count())
        return;
    SheetSettings(this, n - 1).exec();
}

void MainWindow::newProject()
{
    Project *project = new Project;
    Sheet *sheet = new Sheet;
    project->addSheet(sheet);
    projects->addProject(project);
    ui->projectBrowser->setCurrentIndex(projects->getIndex(project));
    if (projects->getActiveProject() == nullptr)
    {
        setActiveProject(project);
        ui->projectBrowser->expand(
                    projects->getIndex(projects->getActiveProject()));
        openSheet(sheet);
    }
}

void MainWindow::open()
{
    QStringList filenames = QFileDialog::getOpenFileNames(
                this, "Open project(s)...",
                getSetting("default_path", QDir::homePath()).toString(),
                "All supported files (*.schim *.xml);;"
                "Schim files (*.schim);;XML files (*.xml);;All files (*)");
    if (filenames.isEmpty())
        return;
    openProjectsFromFiles(filenames);
}

void MainWindow::save()
{
    if (projects->getActiveProject() == nullptr) return;

    if (projects->getActiveProject()->getFileName() == "")
    {
        saveAs();
        return;
    }

    try
    {
        xmlWriteProject(projects->getActiveProject());
    }
    catch (...)
    {
        QMessageBox::critical(this, "Error", "Unable to write file");
    }
}

void MainWindow::saveAs()
{
    if (projects->getActiveProject() == nullptr) return;

    QString filename = QFileDialog::getSaveFileName(this, "Save project...",
                            getSetting("default_path", QDir::homePath()).toString());

    if (filename == "")
        return;

    try
    {
        xmlWriteProject(projects->getActiveProject(), filename);

        projects->getActiveProject()->setFileName(filename);
        changeSetting("default_path", filename);

        setWindowTitle(QString("Schim - ") + projects->getActiveProject()->getName() + " (" + filename + ")");
    }
    catch (...)
    {
        QMessageBox::critical(this, "Error", "Unable to write file");
    }
}

void MainWindow::projectSettings()
{
    if (!projects->isEmpty())
        ProjectSettings().exec();
}

void MainWindow::preferences()
{
    Preferences().exec();
}

void MainWindow::print()
{
    if (projects->getActiveProject() == nullptr) return;

    QString filename = QFileDialog::getSaveFileName(
                this, "Print to file...",
                getSetting(
                    "print_path",
                    QFileInfo(projects->getActiveProject()->getFileName())
                        .absolutePath()).toString()
                );
    if (filename == "") return; // No file chosen

    try
    {
        pdfWriteProject(projects->getActiveProject(), filename);
        changeSetting("print_path", filename);
    }
    catch (...)
    {
        QMessageBox::critical(this, "Error", "Unable to write file");
    }
}

void MainWindow::help()
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

// OVERRIDE QMainWindow

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

// SLOTS

void MainWindow::onTabCloseRequested(int index)
{
    auto *tab = getTab(index);
    ui->tabWidget->removeTab(index);
    if (getTab())
        getTab()->setFocus();
    delete tab;
}

// HELPERS

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
    else if_eq_do("sheet-settings",			sheetSettings)
    else if (action == "project-settings")	projectSettings();
    else return false;
    return true;
}

void MainWindow::setupActions()
{
    // File
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newProject);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::open);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::save);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAs);
    connect(ui->actionProjectSettings, &QAction::triggered,
            this, &MainWindow::projectSettings);
    connect(ui->actionPrint, &QAction::triggered, this, &MainWindow::print);
    connect(ui->actionExit, &QAction::triggered, this, [this](){ close(); });
    // Edit
    connect(ui->actionUndoInSheet, &QAction::triggered, this,
            [this]() { if (getTab()) scene()->undo(); });
    connect(ui->actionRedoInSheet, &QAction::triggered,
            [this]() { if (getTab()) scene()->redo(); });
    connect(ui->actionPreferences, &QAction::triggered,
            this, &MainWindow::preferences);
    // Sheet
    connect(ui->actionNewSheet, &QAction::triggered,
            this, &MainWindow::newSheetAfter);
    // Help
    connect(ui->actionHelp, &QAction::triggered,
            this, &MainWindow::help);
    connect(ui->actionScreenshot, &QAction::triggered,
            this, &MainWindow::takeScreenshot);
    connect(ui->actionDeveloperHints, &QAction::triggered,
            this, &MainWindow::toggleDeveloperHints);
    connect(ui->actionAbout, &QAction::triggered,
            this, [this]() {About(this).exec();});
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
    // Window icon
    setWindowIcon(QIcon(ICON_DIR "/apps/schim.svg"));
#define __ACTIONS SCHIM_ICON_DIR "/actions/"
    ui->actionInsertRect->setIcon(svgColorChange(__ACTIONS "rect.svg"));
    ui->actionInsertLine->setIcon(svgColorChange(__ACTIONS "line.svg"));
#undef __ACTIONS
    ui->actionAbout->setIcon(QIcon(ICON_DIR "/apps/schim.svg"));
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

void MainWindow::openProjectsFromFiles(const QStringList &filenames, int active)
{
    for (const auto &name : filenames)
        projects->addProject(xmlParseProject(name));
    if (active == -1 && projects->rowCount({}) > 1)
        // We are not setting any project as active
        return;
    // We want to set an active project in one of two situations:
    // 1. The user has specified the index of the active project in `active`
    // 2. There are no open projects and only one project is to be opened
    if (active == -1) active = 0; // User didn't specify an active project
    setActiveProject(projects->getProjects()[active]);
    ui->projectBrowser->expand(
                projects->getIndex(projects->getActiveProject()));
    for (auto *sheet : projects->getActiveProject()->getSheets())
        openSheet(sheet);
}

int MainWindow::openSheet(Sheet *sheet, int index)
{
    for (int i = 0; i < ui->tabWidget->count(); ++i)
    {
        auto *tab = (SheetView*) ui->tabWidget->widget(i);
        if (tab->scene()->getSheet() == sheet)
            return i;
    }
    auto *newTab = new SheetView(sheet, ui->tabWidget);
    QString name = sheet->getName();
    if (index == -1)
        getTabWidget()->addTab(newTab, name);
    else
        getTabWidget()->insertTab(index, newTab, name);

    // A tooltip over the central widget is only useful before the user opens
    // any sheets
    ui->centralwidget->setToolTip("");
    return index != -1 ? index : getTabWidget()->count() - 1;
}

void MainWindow::closeSheet(Sheet *sheet)
{
    for (int i = 0; i < getTabWidget()->count(); ++i)
        if (getSheet(i) == sheet)
            onTabCloseRequested(i--);
}

void MainWindow::closeProject(Project *project)
{
    // Remove the project from the Project Browser
    projects->removeProject(project);
    // Close tabs belonging to the project that's being closed
    closeSheetsForProject(project);

    // If only one project is left, set it as the active one
    if (projects->rowCount({}) == 1)
        setActiveProject(projects->getProjects()[0]);
    else
        setActiveProject(nullptr);
    delete project;
}

void MainWindow::closeSheetsForProject(Project *project)
{
    if (project == nullptr) return;
    for (int i = 0; i < getTabWidget()->count(); ++i)
        if (project->getSheets().contains(getSheet(i)))
            // The sheet belongs to `project`
            onTabCloseRequested(i--); // Close tab and delete SheetView
}
