#ifndef PROJECT_H
#define PROJECT_H

#include "sheet.h"

#include <QList>
#include <QString>

/**
 * A project is essentially a list of sheets, along with some project properties.
 * This class should always take care of the destruction of sheets.
 */
class Project : public QList<Sheet*>
{
    QString title{"Untitled Project"};
public:
    Project();
    Project(const QList<Sheet*> &sheets);

    ~Project();

    // Getters

    QString getTitle() const;

    void setTitle(const QString &title);
};

#endif // PROJECT_H
