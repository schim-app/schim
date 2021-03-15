#ifndef PREFERENCES_H
#define PREFERENCES_H

#include "ui/windows/abstractsettingsdialog.h"

#include <QWidget>

namespace Ui { class Preferences; }

class QComboBox;

class Preferences : public AbstractSettingsDialog
{
    Q_OBJECT

public:
    explicit Preferences(QWidget *parent = nullptr);
    ~Preferences();

    void apply() override;

private slots:
    // HELPERS
    void loadConfig();
    void loadGlobalConfig();
    void loadLocalConfig();

private:
    // ATTRIBUTES
    Ui::Preferences *ui;
    QComboBox *comboLocalGlobal{};
};

#endif // PREFERENCES_H
