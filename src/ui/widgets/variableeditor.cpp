#include "variableeditor.h"

#include "ui_variableeditor.h"

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
    if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
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
            if (var.names.isEmpty())
                var.names = QStringList{value.toString()};
            var.names[0] = value.toString();
        }
        else if (col == 1)
            var.names = (var.names[0] + ' ' + value.toString()).split(' ');
        else if (col == 2)
            var.value = value.toString();
        else if (col == 3)
            var.description = value.toString();
        emit dataChanged(index, index);
        emit changed();
    }
    return true;
}

bool VariableTableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        variables.insert(row, {});
    endInsertRows();
    emit changed();
    return true;
}

bool VariableTableModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    for (int i = 0; i < count; ++i)
        variables.removeAt(i);
    endRemoveRows();
    emit changed();
    return true;
}

void VariableTableModel::setVariables(const VariableSet &variables)
{
    beginResetModel();
    this->variables = variables;
    endResetModel();
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

    model = new VariableTableModel();
    connect(model, &VariableTableModel::changed, this, [&] () {
        emit changed();
    });
    ui->table->setModel(model);
}

VariableEditor::~VariableEditor()
{
    delete ui;
}

void VariableEditor::setVariables(const VariableSet &set)
{
    model->setVariables(set);
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
    model->insertRows(model->rowCount({}), 1, {});
    auto lastRowFirstCell = model->index(model->rowCount({}) - 1, 0);
    ui->table->selectionModel()->select(lastRowFirstCell,
                                        QItemSelectionModel::ClearAndSelect);
    ui->table->edit(lastRowFirstCell);
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
    QItemSelectionModel *select = ui->table->selectionModel();
    if (select->hasSelection())
        for (auto index : select->selectedIndexes())
            model->removeRows(index.row(), 1, {});
}
