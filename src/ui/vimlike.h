#ifndef VIMLIKE_H
#define VIMLIKE_H

// TODO deprecated for now

#include <QMap>
#include <QKeySequence>
#include <QString>

class MainWindow; // To avoid circular dependence

static QMap<QString, QKeySequence> vimlikeBindings = {
    {"New Sheet", QKeySequence(Qt::Key_G, Qt::SHIFT + Qt::Key_A)}
};

void enable(MainWindow &window);

void disable(MainWindow &window);

#endif // VIMLIKE_H
