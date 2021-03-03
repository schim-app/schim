#ifndef SHEETSETTINGS_H
#define SHEETSETTINGS_H

#include <QDialog>

#include "ui/sheetscene.h"

// Must declare prototype because of cyclic include
class MainWindow;

namespace Ui { class SheetSettings; };

class SheetSettings : public QDialog
{
    Q_OBJECT
public:
    SheetSettings(MainWindow *parent, int sheetId = 0);
    ~SheetSettings();

    SheetScene *scene() const;

private slots:

    void onChanged();
    void accept() override;
    void reject() override;

    // Grid
    void on_cbShowGrid_toggled(bool checked);
    void on_cbSnapGrid_toggled(bool checked);
    void on_sbGridX_valueChanged(double value);
    void on_sbGridY_valueChanged(double value);

    void on_btnHeader_triggered(QAction *action);

    void on_btnClearHeader_clicked();

    void on_editHeader_textEdited(const QString &text);

private:
    Ui::SheetSettings *ui;
    MainWindow *parent;
    int sheetId;
    bool _changed = false; // Tracks whether any modification was made
    QString originalHeader;
};

#endif // SHEETSETTINGS_H
