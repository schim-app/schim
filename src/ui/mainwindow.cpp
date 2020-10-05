#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "global.h"
#include "fileio/xml.h"

#include <QDebug> //TODO remove
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabView->clear();

    ui->vimKeyStatus->setText("");

    setupActions();

    connect(ui->tabView, &QTabWidget::tabCloseRequested, this, &MainWindow::tabCloseRequest);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/***********
 * Getters *
 ***********/

QTabWidget *MainWindow::getTabView() const
{
    return ui->tabView;
}

Sheet *MainWindow::getCurrentSheet() const
{
    return ((SheetView*) ui->tabView->currentWidget())->scene()->getSheet();
}

int MainWindow::getTabId() const
{
    return ui->tabView->currentIndex();
}

Sheet *MainWindow::getSheet()
{
    return ((SheetView*) ui->tabView->currentWidget())->scene()->getSheet();
}

/***********
 * Setters *
 ***********/

void MainWindow::setTabId(int id)
{
    ui->tabView->setCurrentIndex(id);
}

/********************
 * Events and slots *
 ********************/

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    // Try to interpret input as a vim number
    if (vimEnabled)
        vimNumberFromKeyEvent(event);
}

void MainWindow::tabCloseRequest(int index)
{
    delete ui->tabView->widget(index);
    ui->tabView->removeTab(index);
}

void MainWindow::anyActionTriggered()
{
    resetVimStatus();
}

/*********************
 * Action processing *
 *********************/

void MainWindow::nextTab()
{
    vimNumberConstrain(100);
    vimdo setTabId(getTabId() + 1);
}

void MainWindow::prevTab()
{
    vimNumberConstrain(100);
    vimdo setTabId(getTabId() - 1);
}

void MainWindow::appendSheet()
{
    if (!activeProject)
        return;

    vimNumberConstrain(50);
    vimdo {
        Sheet *sheet = new Sheet;
        activeProject->append(sheet);
        ui->tabView->addTab(new SheetView(sheet), "New sheet");
    }
}

void MainWindow::newSheetBefore()
{
    if (!activeProject)
        return;

    vimNumberConstrain(50);
    vimdo {
        Sheet *sheet = new Sheet;
        activeProject->insert(getTabId(), sheet);
        ui->tabView->insertTab(getTabId(), new SheetView(sheet), "New sheet");
    }
}

void MainWindow::newSheetAfter()
{
    if (!activeProject)
        return;

    vimNumberConstrain(50);
    vimdo {
        Sheet *sheet = new Sheet;
        activeProject->insert(getTabId() + 1, sheet);
        ui->tabView->insertTab(getTabId() + 1, new SheetView(sheet), "New sheet");
    }
}

void MainWindow::closeTab()
{
    if (_vimNumber == 0)
        _vimNumber = getTabId() + 1;
    ui->tabView->removeTab(vimNumber() - 1);
}

void MainWindow::openSheetSettings()
{
    if (_vimNumber == 0)
        _vimNumber = getTabId() + 1;

    if (vimNumber() > ui->tabView->count())
        return;
    sheetSettings.setSheetId(vimNumber() - 1);
    sheetSettings.show();
}

void MainWindow::newProject()
{
    // TODO appropriately dispose of the previously active project
    delete activeProject;
    // Set the new project as active
    activeProject = new Project();
    // Create a new sheet and add it to the project
    Sheet *firstSheet = new Sheet;
    activeProject->append(firstSheet);

    populateWithProject();
}

void MainWindow::openProject()
{
    QString filename = QFileDialog::getOpenFileName(this, "Open project file...",
                                         getSetting("default_path", QDir::homePath()).toString());
    // TODO add filters later
    if (filename == "")
        return;

    changeSetting("default_path", filename);

    try {
        activeProject = xmlParseProject(filename);
        populateWithProject();
    }  catch (std::exception &e) {
        QMessageBox::critical(this, "Error", "Unable to read file");
    }
}

/**********************
 * Vim-specific stuff *
 **********************/

void MainWindow::resetVimStatus()
{
    _vimNumber = 0;
    ui->vimKeyStatus->setText("");
}

void MainWindow::vimNumberFromKeyEvent(QKeyEvent *event)
{
    bool convertSuccess = true;
    // Try to convert the key name to an integer
    int number = QKeySequence(event->key()).toString().toInt(&convertSuccess);

    if (convertSuccess && (_vimNumber != 0 || number != 0) && event->modifiers() == 0)
    { // No modifiers => A number was entered instead of another character
        _vimNumber = _vimNumber * 10 + number;
        ui->vimKeyStatus->setText(QString::number(_vimNumber));
    }

    if (event->key() == Qt::Key_Escape)
        resetVimStatus();
}

int MainWindow::vimNumber()
{
    return qMax(_vimNumber, 1);
}

void MainWindow::vimNumberConstrain(int min, int max)
{
    _vimNumber = qMax(min, qMin(_vimNumber, max));
}

void MainWindow::vimNumberConstrain(int max)
{
    _vimNumber = qMin(_vimNumber, max);
}

/*****************
 * Miscellaneous *
 *****************/

void MainWindow::setupActions()
{
    // Actions that are already defined in the ui file are added to the bottom
    additionalActions = decltype(additionalActions) {
        { new QAction("Prev tab", this), {}, {Qt::SHIFT + Qt::Key_J}, &MainWindow::prevTab },
        { new QAction("Next tab", this), {},  {Qt::SHIFT + Qt::Key_K}, &MainWindow::nextTab },
        { new QAction("New sheet before", this), {},  {Qt::Key_G, Qt::Key_I}, &MainWindow::newSheetBefore },
        { new QAction("New sheet after", this), {},  {Qt::Key_G, Qt::Key_A}, &MainWindow::newSheetAfter },
        { new QAction("Close tab", this), {},  {Qt::Key_G, Qt::Key_D}, &MainWindow::closeTab },
        { new QAction("Sheet settings", this), {},  {Qt::CTRL + Qt::Key_G}, &MainWindow::openSheetSettings },
        { ui->actionNew, {},  {}, &MainWindow::newProject},
        { ui->actionOpen, {},  {}, &MainWindow::openProject},
        { ui->actionNewSheet, {},  {Qt::Key_G, Qt::SHIFT + Qt::Key_A}, &MainWindow::appendSheet},
    };
    if (vimEnabled)
        for (auto action : additionalActions)
        {
            auto qaction = std::get<0>(action); // Alias
            addAction(qaction);
            // Add the vim keybinding as a shortcut to the action
            auto shortcuts = qaction->shortcuts();
            shortcuts.push_back(std::get<2>(action));
            qaction->setShortcuts(shortcuts);
            if (std::get<3>(action) != nullptr)
                connect(qaction, &QAction::triggered, this, std::get<3>(action));
            // Connect the action's triggered signal to a universal slot
            // -- has to be connected AFTER the command-specific slot
            connect(qaction, &QAction::triggered, this, &MainWindow::anyActionTriggered);
        }
}

void MainWindow::populateWithProject()
{
    ui->tabView->clear();
    for (Sheet *sheet : *activeProject)
        ui->tabView->addTab(new SheetView(sheet), sheet->getTitle());
}

void MainWindow::clearTabs()
{
    for (int i = 0; i < ui->tabView->count(); ++i)
        delete ui->tabView->widget(i);
    ui->tabView->clear();
}
