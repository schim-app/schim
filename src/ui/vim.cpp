#include "vim.h"

#include "mainwindow.h"

#include <iostream>
#include <QDebug>

// Private attributes
namespace
{

bool enabled = false;

static QMap<QString, QString> actionMap = {
    // Sheet scene
    {"h", "left"},
    {"j", "down"},
    {"k", "up"},
    {"l", "right"},
    // MainWindow
    {"K", "tab-next"},
    {"J", "tab-prev"},
    {"gd", "tab-close"},
    {"gi", "new-sheet-before"},
    {"ga", "new-sheet-after"},
    {"gA", "sheet-append"},
    // Sheet View
    {"zi", "zoom-in"},
    {"zo", "zoom-out"},
    {"zz", "zoom-set"},
    {"<C-h>", "scroll-left"},
    {"<C-j>", "scroll-down"},
    {"<C-k>", "scroll-up"},
    {"<C-l>", "scroll-right"},
    // Sheet Scene
    {"g+", "grid-increase"},
    {"g-", "grid-decrease"},
    {"i", "insert"},
    // Util
    {"st", "select-texts"},
    {"sp", "select-primitive"},
    // Settings
    {"<C-g>", "sheet-settings"}
    // NEW (update)

};
static QString fullStroke(10);
static QString bareStroke;
static int count = 0;
bool _gettingNumber = true;

void appendNumber(int digit)
{
    count = count * 10 + digit;
}

void updateStroke(const QKeyEvent &event)
{
    QString text = event.text();
    if (text.isEmpty())
        return;
    // fullStroke contains every key press
    if (_gettingNumber)
    {
        if (QRegExp("[1-9]").exactMatch(text)
                || (count > 0 && text == "0")) // The user pressed a number
        {
            if (count < 99999)
                appendNumber(text.toInt());
            else
                return;
        }
        else
            _gettingNumber = false;
    }
    fullStroke += text;
    if (!_gettingNumber)
        // bareStroke contains everything except numbers and motions
        bareStroke += text;

    if (Qt::Key(event.key()) == Qt::Key_unknown)
        return;
}

}

////////////////////////////////////////////////////////////////////////////////
Vim::Count::Count(int count)
    : count(count) { }

Vim::Count::operator int() const
{
    return count == 0 ? 1 : count;
}

int Vim::Count::raw() const
{
    return count;
}

////////////////////////////////////////////////////////////////////////////////

Vim::Action::Action(const QString &command, int count)
    : count(count), command(command) { }

QString Vim::Action::getCommand() const
{
    return command;
}

Vim::Count Vim::Action::getCount() const
{
    return count == 0 ? 1 : count;
}

bool Vim::Action::hasCount() const
{
    return count != 0;
}

bool Vim::Action::operator==(const QString &cmd) const
{
    return getCommand() == cmd;
}

////////////////////////////////////////////////////////////////////////////////

void Vim::enable(bool enable)
{
    enabled = enable;
}

Vim::Count Vim::n()
{
    return Count(count);
}

void Vim::registerKeyPress(QKeyEvent *event,
                           std::function<bool(const Vim::Action &)> callback)
{
    // TODO bug This will update stroke twice if the child item does not
    // accept the event
    // Actually this seems to not be necessary -- do some testing
    QString fullStroke_old = fullStroke, bareStroke_old = bareStroke;
    int count_old = count;
    updateStroke(*event);
    event->accept();
    for (auto it = actionMap.begin(); it != actionMap.end(); ++it)
    {
        if (it.key() == bareStroke)
        { // The input stroke matches a binding
            Action action(it.value(), count);
            if (!callback(action))
            { // Caller does not want the event
                fullStroke = std::move(fullStroke_old);
                bareStroke = std::move(bareStroke_old);
                count = count_old;
                event->ignore();
                return;
            }
            break;
        }
        else if (it.key().startsWith(bareStroke))
        {
            MainWindow::getInstance()->setVimStatus(fullStroke);
            return;
        }
    }
    resetStroke();
    MainWindow::getInstance()->setVimStatus(fullStroke);
}

void Vim::addBinding(const QString &sequence, const QString &action)
{
    actionMap[sequence] = action;
}

void Vim::addBindings(const QMap<QString, QString> &map)
{
    actionMap.insert(map);
}

void Vim::resetStroke()
{
    fullStroke = bareStroke = "";
    // When the user starts the next stroke, vim will try to interpret the first
    // keypresses as counts.
    _gettingNumber = true;
    count = 0;
}

QString Vim::getStatusText()
{
    return fullStroke;
}
