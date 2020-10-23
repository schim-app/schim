#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "global.h"
#include "fileio/xml.h"
#include "ui/objects/gline.h"
#include "ui/objects/gcompositeobject.h"
#include "ui/operations.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QScrollBar>


MainWindow* MainWindow::instance{};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabView->clear();

    ui->vimKeyStatus->setText("");

    setupActions();

    connect(ui->tabView, &QTabWidget::tabCloseRequested, this, &MainWindow::tabCloseRequest);

    instance = this;
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

int MainWindow::getTabId() const
{
    return ui->tabView->currentIndex();
}

SheetView *MainWindow::getTab()
{
    return (SheetView*) ui->tabView->currentWidget();
}

SheetScene *MainWindow::scene()
{
    return getTab() ? getTab()->scene() : nullptr;
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
        ui->tabView->addTab(new SheetView(sheet, ui->tabView), "New sheet");
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
        ui->tabView->insertTab(getTabId(), new SheetView(sheet, ui->tabView), "New sheet");
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
        ui->tabView->insertTab(getTabId() + 1, new SheetView(sheet, ui->tabView), "New sheet");
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

    try {
        activeProject = xmlParseProject(filename);

        this->filename = filename;
        changeSetting("default_path", filename);

        setWindowTitle(QString("Schim - ") + activeProject->getTitle() + " (" + filename + ")");
        populateWithProject();
    }  catch (std::exception &e) {
        QMessageBox::critical(this, "Error", "Unable to read file");
    }
}

void MainWindow::save()
{
    if (activeProject == nullptr) return;

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

void MainWindow::zoomIn()
{
    vimdo if (getTab()) getTab()->zoomIn();
}

void MainWindow::zoomOut()
{
    vimdo if (getTab()) getTab()->zoomOut();
}

void MainWindow::setZoom()
{
    if (getTab() == nullptr) return;
    if (_vimNumber == 0)
        getTab()->resetZoom();
    else
        getTab()->setZoom(vimNumber() / 100.);
}

void MainWindow::scrollUp()
{
    if (getTab() == nullptr) return;
    vimdo {
        auto *bar = getTab()->verticalScrollBar();
        bar->setValue(bar->value() - bar->singleStep());
    }
    getTab()->viewport()->update();
}

void MainWindow::scrollDown()
{
    if (getTab() == nullptr) return;
    vimdo {
        auto *bar = getTab()->verticalScrollBar();
        bar->setValue(bar->value() + bar->singleStep());
    }
    getTab()->viewport()->update();
}

void MainWindow::scrollLeft()
{
    if (getTab() == nullptr) return;
    vimdo {
        auto *bar = getTab()->horizontalScrollBar();
        bar->setValue(bar->value() - bar->singleStep());
    }
    getTab()->viewport()->update();
}

void MainWindow::scrollRight()
{
    if (getTab() == nullptr) return;
    vimdo {
        auto *bar = getTab()->horizontalScrollBar();
        bar->setValue(bar->value() + bar->singleStep());
    }
    getTab()->viewport()->update();
}

void MainWindow::undoInSheet()
{
    if (getTab())
        scene()->undo();
}

void MainWindow::redoInSheet()
{
    if (getTab())
        scene()->redo();
}

void MainWindow::insertLine()
{
    if (getTab())
        scene()->startOperation(new LineInsertOperation(scene()));
}

void MainWindow::insertRect()
{
    if (getTab())
        scene()->startOperation(new RectInsertOperation(scene()));
}

void MainWindow::insertText()
{
    if (getTab())
        scene()->startOperation(new TextInsertOperation(scene()));
}

void MainWindow::increaseGridSize()
{
    if (getTab())
    {
        scene()->setGridSize(scene()->getGridSize() + QSizeF{1, 1});
        scene()->update();
    }
}

void MainWindow::decreaseGridSize()
{
    if (getTab())
    {
        scene()->setGridSize(scene()->getGridSize() - QSizeF{1, 1});
        scene()->update();
    }
}

void MainWindow::showAllTexts()
{
    if (getTab())
    {
        for (auto *obj : scene()->items())
            if (dynamic_cast<GText*>(obj))
                obj->setSelected(true);
    }
}

void MainWindow::showAllPrimitives()
{
    if (getTab())
    {
        for (auto *obj : scene()->items())
            if (!dynamic_cast<GCompositeObject*>(obj))
                obj->setSelected(true);
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
        { new QAction("Zoom in", this), {Qt::CTRL + Qt::Key_Plus}, {Qt::Key_Z, Qt::Key_I}, &MainWindow::zoomIn},
        { new QAction("Zoom out", this), {Qt::CTRL + Qt::Key_Minus}, {Qt::Key_Z, Qt::Key_O}, &MainWindow::zoomOut},
        { new QAction("Reset zoom", this), {Qt::CTRL + Qt::Key_0}, {Qt::Key_Z, Qt::Key_Z}, &MainWindow::setZoom},
        { new QAction("Scroll up", this), {}, {Qt::CTRL + Qt::Key_K}, &MainWindow::scrollUp},
        { new QAction("Scroll down", this), {}, {Qt::CTRL + Qt::Key_J}, &MainWindow::scrollDown},
        { new QAction("Scroll left", this), {}, {Qt::CTRL + Qt::Key_H}, &MainWindow::scrollLeft},
        { new QAction("Scroll right", this), {}, {Qt::CTRL + Qt::Key_L}, &MainWindow::scrollRight},
        { new QAction("Increase grid", this), {}, {Qt::Key_G, Qt::Key_Plus}, &MainWindow::increaseGridSize},
        { new QAction("Decrease grid", this), {}, {Qt::Key_G, Qt::Key_Minus}, &MainWindow::decreaseGridSize},
        { new QAction("Show all texts", this), {}, {Qt::Key_S, Qt::Key_T}, &MainWindow::showAllTexts},
        { new QAction("Show all primitives", this), {}, {Qt::Key_S, Qt::Key_P}, &MainWindow::showAllPrimitives},
        { ui->actionNew, {},  {}, &MainWindow::newProject},
        { ui->actionOpen, {},  {}, &MainWindow::openProject},
        { ui->actionSave, {}, {}, &MainWindow::save},
        { ui->actionSaveAs, {}, {}, &MainWindow::saveAs},
        { ui->actionNewSheet, {},  {Qt::Key_G, Qt::SHIFT + Qt::Key_A}, &MainWindow::appendSheet},
        { ui->actionInsertLine, {}, {}, &MainWindow::insertLine},
        { ui->actionInsertRect, {}, {}, &MainWindow::insertRect},
        { ui->actionInsertText, {}, {}, &MainWindow::insertText},
        { ui->actionUndoInSheet, {}, {Qt::Key_U}, &MainWindow::undoInSheet},
        { ui->actionRedoInSheet, {}, {Qt::CTRL + Qt::Key_R}, &MainWindow::redoInSheet},
    };
    if (vimEnabled)
        for (auto action : additionalActions)
        {
            auto qaction = std::get<0>(action); // Alias
            addAction(qaction);

            auto shortcuts = qaction->shortcuts();
            // Add an additional keybinding to the action
            shortcuts.append(std::get<1>(action));
            // Add the vim keybinding as a shortcut to the action
            shortcuts.append(std::get<2>(action));
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
        ui->tabView->addTab(new SheetView(sheet, ui->tabView), sheet->getTitle());
}

void MainWindow::clearTabs()
{
    for (int i = 0; i < ui->tabView->count(); ++i)
        delete ui->tabView->widget(i);
    ui->tabView->clear();
}

void MainWindow::on_todoButton_pressed()
{
    if (getTab() != nullptr)
        getTab()->viewport()->repaint();
}

//TODO remove
void MainWindow::dispValue(const QString &str)
{
    ui->todoButton->setText(str);
}
