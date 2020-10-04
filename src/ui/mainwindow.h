#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QKeyEvent>
#include <QList>

#include "sheetsettings.h"
#include "sheetview.h"
#include "vimlike.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// TODO doc
#define vimdo for (int Intentionally_Long_Unlikely_Name = 0;\
    Intentionally_Long_Unlikely_Name < vimNumber(); ++Intentionally_Long_Unlikely_Name)

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QTabWidget *getTabView() const;
    Sheet *getCurrentSheet() const;

    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void tabCloseRequest(int index);

    void anyActionTriggered();

    void nextTab();
    void prevTab();
    void appendSheet();
    void newSheetBefore();
    void newSheetAfter();
    void closeTab();
    void openSheetSettings();

private: // Private methods

    void resetVimStatus();
    /**
     * Try to interpret the key event as a vim number. On success, the appropriate
     * private variables are modified. On failure, nothing happens.
     */
    void vimNumberFromKeyEvent(QKeyEvent *event);
    /** Return the number of times to perform the next vim command. */
    int vimNumber();
    void vimNumberConstrain(int min, int max);
    void vimNumberConstrain(int max);

private: // Private variables
    Ui::MainWindow *ui;
    int _vimNumber = 0;
    bool vimEnabled = true; //TODO temporarily enabled
    // Actions that cannot be found in the menu + vim-ification of the ones that can
    QList<std::tuple<QAction*, QKeySequence, QKeySequence, void (MainWindow::*)()>> additionalActions;
    SheetSettings sheetSettings{this};

    // Added so SheetSettings can use ui->tabView
    friend class SheetSettings;
};
#endif // MAINWINDOW_H
