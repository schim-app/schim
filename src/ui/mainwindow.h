#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QKeyEvent>
#include <QList>

#include "model/project.h"

#include "ui/windows/sheetsettings.h"
#include "sheetview.h"
#include "ui/vim.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief The main application window.
 *
 * @note This is a singleton class.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    // CONSTRUCTORS
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow *getInstance();

    // GETTERS
    /** Return the tab widget */
    QTabWidget *getTabView() const;
	/** Return the current tab id*/
    int getTabId() const;
    /**
     * @brief Return the tab of the specified index as a `SheetView*`.
     * If no index is specified, return the current tab.
     */
    SheetView *getTab(int index = -1);
    SheetScene *scene();
    /**
     * @brief Return the `Sheet*` that is part of the currently active tab
     * If no index is specified, return the current sheet.
     */
    Sheet *getSheet(int index = -1);
    QString getFileName() const;

    // SETTERS
    void setTabId(int id);
    void setVimStatus(const QString &status);

    // EVENTS
    void keyPressEvent(QKeyEvent *event) override;

    // ACTION PROCESSING
    void nextTab(Vim::N n = 1);
    void prevTab(Vim::N n = 1);
    void appendSheet(Vim::N n);
    void newSheetBefore(Vim::N n = 1);
    void newSheetAfter(Vim::N n = 1);
    void closeTab(Vim::N = 0);
    void openSheetSettings(Vim::N = 0);
    void newProject();
    void openProject();
    void save();
    void saveAs();
    void print();
    void cursorLeft();
    void cursorDown();
    void cursorUp();
    void cursorRight();
    void showHelp();
    void takeScreenshot();
    void toggleDeveloperHints();

    void openProjectFromFile(const QString &filename);

private slots:
    // SLOTS
    void onTabCloseRequested(int index);

private:
    // HELPERS
    bool processVimAction(const Vim::Action &action);
    void setupActions();
    /// Populate the window with a new active project.
    void populateWithProject();
    /// Remove all tabs and destroy corresponding widgets.
    void clearTabs();

private:
    // ATTRIBUTES
    Ui::MainWindow *ui;
    int _vimNumber = 0;
    Project *activeProject = nullptr;
    QString filename;

    static MainWindow *instance;
};
#endif // MAINWINDOW_H
