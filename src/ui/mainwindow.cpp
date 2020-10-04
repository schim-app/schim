#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug> //TODO

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Set default tab name
    ui->tabView->setTabText(0, "New sheet");

    ui->vimKeyStatus->setText("");

    // Actions that are already defined in the ui file are added to the bottom
    additionalActions = decltype(additionalActions) {
        { new QAction("Prev tab", this), {}, {Qt::SHIFT + Qt::Key_J}, &MainWindow::prevTab },
        { new QAction("Next tab", this), {},  {Qt::SHIFT + Qt::Key_K}, &MainWindow::nextTab },
        { new QAction("New sheet before", this), {},  {Qt::Key_G, Qt::Key_I}, &MainWindow::newSheetBefore },
        { new QAction("New sheet after", this), {},  {Qt::Key_G, Qt::Key_A}, &MainWindow::newSheetAfter },
        { new QAction("Close tab", this), {},  {Qt::Key_G, Qt::Key_D}, &MainWindow::closeTab },
        { new QAction("Sheet settings", this), {},  {Qt::CTRL + Qt::Key_G}, &MainWindow::openSheetSettings },
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

    connect(ui->tabView, &QTabWidget::tabCloseRequested, this, &MainWindow::tabCloseRequest);
}

MainWindow::~MainWindow()
{
    delete ui;
}

QTabWidget *MainWindow::getTabView() const
{
    return ui->tabView;
}

Sheet *MainWindow::getCurrentSheet() const
{
    return ((SheetView*) ui->tabView->currentWidget())->scene()->getSheet();
}

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

void MainWindow::nextTab()
{
    vimNumberConstrain(100);
    vimdo ui->tabView->setCurrentIndex(ui->tabView->currentIndex() + 1);
}

void MainWindow::prevTab()
{
    vimNumberConstrain(100);
    vimdo ui->tabView->setCurrentIndex(ui->tabView->currentIndex() - 1);
}

void MainWindow::appendSheet()
{
    vimNumberConstrain(50);
    vimdo ui->tabView->addTab(new SheetView, "New sheet");
}

void MainWindow::newSheetBefore()
{
    vimNumberConstrain(50);
    vimdo ui->tabView->insertTab(ui->tabView->currentIndex(), new SheetView(), "New sheet");
}

void MainWindow::newSheetAfter()
{
    vimNumberConstrain(50);
    vimdo ui->tabView->insertTab(ui->tabView->currentIndex() + 1, new SheetView(), "New sheet");
}

void MainWindow::closeTab()
{
    if (_vimNumber == 0)
        _vimNumber = ui->tabView->currentIndex() + 1;
    ui->tabView->removeTab(_vimNumber - 1);
}

void MainWindow::openSheetSettings()
{
    if (_vimNumber == 0)
        _vimNumber = ui->tabView->currentIndex() + 1;
    sheetSettings.setSheetId(vimNumber() - 1);
    sheetSettings.show();
}

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
