#include "vim.h"

#include "mainwindow.h"

#include <iostream>
#include <QDebug>

// Private attributes
namespace
{

bool enabled = false;

static QMap<QString, QString> actionMap = {
    // General
    {"<C-n>", "next-entry"},
    {"<C-p>", "prev-entry"},
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
    {"u", "undo"},
    {"<C-r>", "redo"},
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

#ifdef Q_OS_WIN32
#	include <windows.h>
#else
#	include <X11/XKBlib.h>
#	undef KeyPress
#	undef KeyRelease
#	undef FocusIn
#	undef FocusOut
#endif

// TODO test on windows
bool checkCapsLock()
{
#ifdef Q_OS_WIN32
    return GetKeyState(VK_CAPITAL) == 1;
#else // X11
    Display *d = XOpenDisplay((char*)0);
    if (d)
    {
        unsigned n;
        XkbGetIndicatorState(d, XkbUseCoreKbd, &n);
        return (n & 0x01) == 1;
    }
    return false;
#endif
}

void appendNumber(int digit)
{
    count = count * 10 + digit;
}

QString keyToString(const QKeyEvent &event)
{
    QString text = "";
    if (event.modifiers() & Qt::CTRL)
    {
        QString seqText = QKeySequence(event.key()).toString();
        text = "<C-" +
                (checkCapsLock() != (event.modifiers() & Qt::SHIFT) ? seqText : seqText.toLower())
                + ">";
    }
    else
        text = event.text();

    return text;
}

void updateStroke(const QKeyEvent &event, bool allowCount)
{
    QString text = keyToString(event);
    if (text.isEmpty())
        return;
    // fullStroke contains every key press
    if (_gettingNumber)
    {
        if (!allowCount)
            _gettingNumber = false;
        else if (QRegExp("[1-9]").exactMatch(text)
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
                           std::function<bool(const Vim::Action &)> callback,
                           bool allowCount)
{
    // TODO remove this?
    if (event->key() == Qt::Key_Control || event->key() == Qt::Key_Shift)
        return;
    // TODO bug This will update stroke twice if the child item does not
    // accept the event
    // Actually this seems to not be necessary -- do some testing
    QString fullStroke_old = fullStroke, bareStroke_old = bareStroke;
    int count_old = count;
    updateStroke(*event, allowCount);
    for (auto it = actionMap.begin(); it != actionMap.end(); ++it)
    {
        if (it.key() == bareStroke)
        { // The input stroke matches a binding
            // Reset it here because callback can potentionally block execution
            MainWindow::getInstance()->setVimStatus("");
            Action action(it.value(), count);
            event->accept();
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
            event->accept();
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
