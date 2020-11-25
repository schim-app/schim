#ifndef TEXTSETTINGS_H
#define TEXTSETTINGS_H

#include <QDialog>

#include "ui/objects/gtext.h"

namespace Ui {
class TextSettings;
}

class TextSettings : public QDialog
{
    Q_OBJECT

public:
    explicit TextSettings(GText *obj, QWidget *parent = nullptr);
    ~TextSettings();

    void reload();

private slots:
    void on_editText_textEdited(const QString &text);
    void on_fontComboBox_currentFontChanged(const QFont &font);

private:
    Ui::TextSettings *ui;
    GText *obj;
};

#endif // TEXTSETTINGS_H
