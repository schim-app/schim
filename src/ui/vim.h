#ifndef VIM_H
#define VIM_H

#include <QKeyEvent>

/**
  * Repeat a command a number of times, as determined by vimNumber().
  * Usage: vimdo single_command
  * 	   vimdo { multiple_commands }
  */
#define vimdo(N) for (int Intentionally_Long_Unlikely_Name = 0;\
    Intentionally_Long_Unlikely_Name < N; ++Intentionally_Long_Unlikely_Name)

namespace Vim
{

class Count
{
    int count;
public:
    Count(int count);

    operator int() const;
    int raw() const;
};

typedef Count N;

class Action
{
    // A count of 0 means that no count was specified
    int count = 0;
    // in_reg = -1 means that the stroke takes no input register
    char in_reg = -1;
    QString command;
    //TODO motion;

public:
    Action(const QString &command, int count = 0);

    QString getCommand() const;
    Count getCount() const;

    bool hasCount() const;

    bool operator==(const QString &cmd) const;
};

void enable(bool enable);
Count n();

void addBinding(const QString &sequence, const QString &action);
void addBindings(const QMap<QString, QString> &map);
void registerKeyPress(QKeyEvent *event,
                      std::function<bool(const Action &action)> callback);
void resetStroke();

QString getStatusText();
}

#endif // VIM_H
