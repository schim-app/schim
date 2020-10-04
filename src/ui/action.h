#ifndef ACTION_H
#define ACTION_H

#include <QAction>

class Action : QAction
{
public:
    Action();

    bool event(QEvent *event) override;
};

#endif // ACTION_H
