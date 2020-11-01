#ifndef SHEETSETTINGS_H
#define SHEETSETTINGS_H

#include <QDialog>

// Must declare prototype because of cyclic include
class MainWindow;

namespace Ui { class SheetSettings; };

class SheetSettings : public QDialog
{
public:
    SheetSettings(MainWindow *parent, int sheetId = 0);
    ~SheetSettings();

    void setSheetId(int id);

    void show();

private slots:
    void accept() override;

private:
    Ui::SheetSettings *ui;
    MainWindow *parent;
    int sheetId;

};

#endif // SHEETSETTINGS_H
