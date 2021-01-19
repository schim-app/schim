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

    void setSheetId(int id);
    SheetScene *scene() const;

    void show();

private slots:

    void onChanged();
    void accept() override;
    void reject() override;

    // Grid
    void on_cbShowGrid_toggled(bool checked);
    void on_cbSnapGrid_toggled(bool checked);
    void on_sbGridX_valueChanged(double value);
    void on_sbGridY_valueChanged(double value);

private:
    Ui::SheetSettings *ui;
    MainWindow *parent;
    int sheetId;
    bool changed; // Tracks whether any modification was made

};

#endif // SHEETSETTINGS_H
