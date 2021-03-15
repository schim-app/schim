#ifndef VARIABLEEDITOR_H
#define VARIABLEEDITOR_H

#include "model/variable.h"

#include <QStandardItem>
#include <QTableView>
#include <QWidget>

namespace Ui {
class VariableEditor;
}

typedef QStandardItem VariableItem;

class VariableTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit VariableTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);

    bool insertRows(int row, int count, const QModelIndex &parent);
    bool removeRows(int row, int count, const QModelIndex &parent);

    void setVariables(const VariableSet &variables);
    VariableSet getVariables() const;

signals:
    void changed();
private:
    VariableSet variables;
};

class VariableTableView : public QTableView
{
public:
    explicit VariableTableView(QWidget *parent = nullptr);
};

class VariableEditor : public QWidget
{
    Q_OBJECT

public:
    explicit VariableEditor(QWidget *parent = nullptr);
    ~VariableEditor();

    void setVariables(const VariableSet &var);
    VariableSet getVariables() const;

    bool eventFilter(QObject* obj, QEvent *event) override;
    void timerEvent(QTimerEvent *event) override;

signals:

    void changed();
    void focused();

private:

    void updateVariableName();

private slots:

    void on_btnAddVariable_clicked();
    void on_btnRemoveVariable_clicked();

private:
    Ui::VariableEditor *ui;
    VariableTableModel *model;
};

#endif // VARIABLEEDITOR_H
