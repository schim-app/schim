#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QKeyEvent>
#include <QList>

#include <model/project.h>

#include "sheetsettings.h"
#include "sheetview.h"
#include "vimlike.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
  * Repeat a command a number of times, as determined by vimNumber().
  * Usage: vimdo single_command
  * 	   vimdo { multiple_commands }
  */
#define vimdo for (int Intentionally_Long_Unlikely_Name = 0;\
    Intentionally_Long_Unlikely_Name < vimNumber(); ++Intentionally_Long_Unlikely_Name)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // Getters

    QTabWidget *getTabView() const;
    Sheet *getCurrentSheet() const;
    int getTabId() const;
    Sheet *getSheet();

    // Setters

    void setTabId(int id);

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void tabCloseRequest(int index);

    // Action processing

    void anyActionTriggered();
    void nextTab();
    void prevTab();
    void appendSheet();
    void newSheetBefore();
    void newSheetAfter();
    void closeTab();
    void openSheetSettings();
    void newProject();
    void openProject();

private: // Private methods

    // Vim-specific stuff

    void resetVimStatus();
    /**
     * Try to interpret the key event as a vim number. On success, the appropriate
     * private variables are modified. On failure, nothing happens.
     */
    void vimNumberFromKeyEvent(QKeyEvent *event);
    /** Return the number of times the next vim command should be performed. */
    int vimNumber();
    void vimNumberConstrain(int min, int max);
    void vimNumberConstrain(int max);

    // Miscellaneous

    void setupActions();
    /** Populate the window with a new active project */
    void populateWithProject();
    /** Remove all tabs and destroy corresponding widgets */
    void clearTabs();

private: // Private variables
    Ui::MainWindow *ui;
    int _vimNumber = 0;
    bool vimEnabled = true; //TODO temporarily enabled
    // Actions that cannot be found in the menu + vim-ification of the ones that can
    QList<std::tuple<QAction*, QKeySequence, QKeySequence, void (MainWindow::*)()>> additionalActions;
    SheetSettings sheetSettings{this};
    Project *activeProject = nullptr;

    // Added so SheetSettings can use ui->tabView
    friend class SheetSettings;
};
#endif // MAINWINDOW_H
