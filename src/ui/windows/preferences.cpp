#include "preferences.h"
#include "ui_preferences.h"

#include "global.h"

#include <QComboBox>
#include <QLabel>

Preferences::Preferences(QWidget *parent) :
    AbstractSettingsDialog(parent),
    ui(new Ui::Preferences)
{
    addCategory("General");
    addCategory("Input");
    addCategory("Tweaks");
    ui->setupUi(getStackedWidget());

    initialize();

    // Combo box
    comboLocalGlobal = new QComboBox(this);
    comboLocalGlobal->addItem("Global");
    comboLocalGlobal->addItem("Local");
    getTitleLayout()->addSpacerItem(new QSpacerItem(20, 1, QSizePolicy::Expanding));
    getTitleLayout()->addWidget(new QLabel("Configuration file:", this));
    getTitleLayout()->addWidget(comboLocalGlobal);

    loadConfig();
}

Preferences::~Preferences()
{
    delete ui;
}

void Preferences::apply()
{
    changeSetting("UI/enableVimMode", ui->cbVimMode->isChecked());
    Vim::enable(ui->cbVimMode->isChecked());
}

void Preferences::loadConfig()
{
    if (comboLocalGlobal->currentIndex() == 0)
        loadLocalConfig();
    if (comboLocalGlobal->currentIndex() == 1)
        loadGlobalConfig();
}

void Preferences::loadGlobalConfig()
{
    ui->cbVimMode->setChecked(getSetting("UI/enableVimMode", false).toBool());
}

void Preferences::loadLocalConfig()
{

}
