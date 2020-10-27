#ifndef COMPONENTLIST_H
#define COMPONENTLIST_H

#include <QStandardItemModel>
#include <QTreeView>

class ComponentList : public QTreeView
{
public:
    ComponentList(QWidget *parent = nullptr);

    /**
     * @brief Populate the list with items from the system location.
     */
    void synchronize();

    void iterate(const QString &dir, QStandardItem *parent = nullptr);

    QStandardItem *createItem(const QString &name);
    QStandardItem *createDir(const QString &name = "");

private:
    QStandardItemModel *model;
};

#endif // COMPONENTLIST_H
