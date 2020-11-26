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
    void accept() override;

    // Grid
    void on_cbShowGrid_toggled(bool checked);
    void on_cbSnapGrid_toggled(bool checked);
    void on_sbGridX_valueChanged(double value);
    void on_sbGridY_valueChanged(double value);

private:
    Ui::SheetSettings *ui;
    MainWindow *parent;
    int sheetId;

};

#endif // SHEETSETTINGS_H
