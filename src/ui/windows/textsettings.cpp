#include "textsettings.h"
#include "ui_textsettings.h"

TextSettings::TextSettings(GText *obj, QWidget *parent)
    : QDialog(parent), ui(new Ui::TextSettings), obj(obj)
{
    ui->setupUi(this);
    reload();
}

TextSettings::~TextSettings()
{
    delete ui;
}

void TextSettings::reload()
{
    ui->editText->setText(obj->get()->getText());
    QFont font;
    font.fromString(obj->get()->getFont());
    ui->fontComboBox->setCurrentFont(font);
}

void TextSettings::on_editText_textEdited(const QString &text)
{
    obj->get()->setText(text);
    obj->reloadFromModel();
}

void TextSettings::on_fontComboBox_currentFontChanged(const QFont &font)
{
    obj->get()->setFont(font.toString());
    obj->reloadFromModel();
}
