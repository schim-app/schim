#include "variableeditor.h"

#include "ui_variableeditor.h"

/****************
 * VariableItem *
 ****************/

/**********************
 * VariableTableModel *
 **********************/

VariableTableModel::VariableTableModel(QObject *parent)
{ }

int VariableTableModel::rowCount(const QModelIndex &parent) const
{
    return variables.size();
}

int VariableTableModel::columnCount(const QModelIndex &parent) const
{
    return 4;
}

QVariant VariableTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return {};
    auto var = variables[index.row()];
    switch (index.column())
    {
        case 0: return var.getTrueName(); break;
        case 1: return var.getAliases().join(' '); break;
        case 2: return var.value; break;
        case 3: return var.description; break;
    }
    return {};
}

QVariant VariableTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};
    if (orientation == Qt::Horizontal)
    {
        if (section == 0) return "Name";
        if (section == 1) return "Aliases";
        if (section == 2) return "Value";
        if (section == 3) return "Description";
        return {};
    }
    else
        return section + 1;
}

Qt::ItemFlags VariableTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid()) return Qt::ItemIsEnabled;
    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool VariableTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid() && role == Qt::EditRole)
    {
        auto &var = variables[index.row()];
        int col = index.column();
        if (col == 0)
        {
            // TODO
        }
        emit dataChanged(index, index);
    }
    return true;
}

bool VariableTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        variables.insert(row, {});
    endInsertRows();
    return true;
}

void VariableTableModel::append()
{
    insertRows(variables.size(), 1, {});
}

void VariableTableModel::setVariables(const VariableSet &variables)
{
    beginInsertRows({}, 0, variables.size() - 1);
    this->variables = variables;
    endInsertRows();
}

VariableSet VariableTableModel::getVariables() const
{
    return variables;
}

/*********************
 * VariableTableView *
 *********************/

VariableTableView::VariableTableView(QWidget *parent)
    : QTableView(parent)
{
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //TODO verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

/******************
 * VariableEditor *
 ******************/

VariableEditor::VariableEditor(QWidget *parent)
    : QWidget(parent), ui(new Ui::VariableEditor)
{
    ui->setupUi(this);

    for (auto *child : children())
        child->installEventFilter(this);

    reload();

    model = new VariableTableModel();
    ui->table->setModel(model);
}

VariableEditor::~VariableEditor()
{
    delete ui;
}

void VariableEditor::reload()
{
    /*
    auto nameAbbrevList = variable.name.split(" ");
    if (!nameAbbrevList.empty())
        ui->editName->setText(nameAbbrevList[0]);
    else
        ui->editName->clear();

    ui->editAbbreviations->setText(QString(variable.name).replace(QRegExp("^[^ ]{1,} "), ""));
    ui->editValue->setText(variable.value);
    ui->editDescription->setText(variable.description);
    */
}

void VariableEditor::setVariables(const VariableSet &set)
{
    model->setVariables(set);
    reload();
}

VariableSet VariableEditor::getVariables() const
{
    return model->getVariables();
}

bool VariableEditor::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::FocusIn)
        emit focused();
    return QWidget::eventFilter(obj, event);
}

void VariableEditor::timerEvent(QTimerEvent *event)
{
    QWidget::timerEvent(event);
    //TODO
    //auto *scrollBar = ui->scrollArea->verticalScrollBar();
    //scrollBar->setSliderPosition(scrollBar->maximum());
}

void VariableEditor::updateVariableName()
{
    /*
    variable.name = ui->editName->text();
    if (!ui->editAbbreviations->text().isEmpty())
        foreach (auto str, ui->editAbbreviations->text().split(' '))
            variable.name += QString(" ") + str;
            */
}

void VariableEditor::on_btnAddVariable_clicked()
{
    static int timerId = -1;
    model->append();
    emit changed();

    if (timerId != -1)
    {
        killTimer(timerId);
        timerId = -1;
    }
    timerId = startTimer(150);
}

void VariableEditor::on_btnRemoveVariable_clicked()
{
    /*
    if (focusedEditor)
    { // Remove the corresponding editor
        variableEditors.removeOne(focusedEditor);
        ui->variablesView->layout()->removeWidget(focusedEditor);
        delete focusedEditor;
    }

    focusedEditor = nullptr;
    changed = true;

    updateDesignatorFields();
    */
}

void VariableEditor::on_editName_textEdited(const QString &text)
{
    Q_UNUSED(text)
    /*
    // If the name has been typed in for the first time, create an abbreviation
    // that consists of the first character of the name
    if (text.size() == 1 && ui->editAbbreviations->text().isEmpty())
    {
        variable.name = text + " " + text[0];
        reload();
    }
    else
        updateVariableName();
    emit changed();
    */
}

void VariableEditor::on_editAbbreviations_textEdited(const QString &text)
{
    /*
    Q_UNUSED(text)
    updateVariableName();
    emit changed();
    */
}
