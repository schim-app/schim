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
    {"d", "delete"}, // TODO impl
    {"x", "cut"}, // TODO impl
    // Sheet scene
    {"h", "left"},
    {"j", "down"},
    {"k", "up"},
    {"l", "right"},
    // MainWindow
    {"K", "tab-next"},
    {"J", "tab-prev"},
    {"g0", "tab-first"},
    {"g$", "tab-last"},
    {"gd", "tab-close"},
    {"gi", "new-sheet-before"},
    {"ga", "new-sheet-after"},
    {"gA", "sheet-append"},
    {"go", "sheet-open"}, // TODO impl
    {"gO", "sheet-open-all"}, // TODO impl
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
    {"<C-g>", "sheet-settings"},
    {"g<C-g>", "project-settings"},
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

} // namespace

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

bool Vim::Action::operator==(const QString &cmd) const
{
    return getCommand() == cmd;
}

////////////////////////////////////////////////////////////////////////////////

void Vim::enable(bool enable)
{
    ::enabled = enable;
}

bool Vim::enabled()
{
    return ::enabled;
}

Vim::Count Vim::n()
{
    return Count(count);
}

void Vim::registerKeyPress(QKeyEvent *event,
                           std::function<bool(const Vim::Action &)> callback,
                           bool allowCount)
{
    if (!::enabled) return;
    // TODO remove this?
    if (event->key() == Qt::Key_Control || event->key() == Qt::Key_Shift)
        return;
    QString fullStroke_old = fullStroke, bareStroke_old = bareStroke;
    int count_old = count;
    // Update the status with the key sequence obtained from `event`
    updateStroke(*event, allowCount);
    for (auto it = actionMap.begin(); it != actionMap.end(); ++it)
    {
        if (it.key() == bareStroke) // The input stroke matches a binding
        {
            // Reset it here because callback can potentionally block execution
            MainWindow::getInstance()->setVimStatus("");
            Action action(it.value(), count);
            if (!callback(action))
            { // Caller does not want the event
                // Undo any changes
                fullStroke = std::move(fullStroke_old);
                bareStroke = std::move(bareStroke_old);
                count = count_old;
                // This widget does not want the event - ignore the event
                // to let parent widgets process this
                event->ignore();
                return; // We do not want to resetStroke just yet
            }
            else event->accept();
            break;
        }
        else if (it.key().startsWith(bareStroke))
        {
            MainWindow::getInstance()->setVimStatus(fullStroke);
            // Vim status has been updated, don't let parent widgets do the same
            event->accept();
            return;
        }
    }
    // The key sequence is wholly unrecognized -- discard it
    resetStroke();
    MainWindow::getInstance()->setVimStatus(fullStroke);
}

void Vim::addBinding(const QString &sequence, const QString &action)
{
    actionMap[sequence] = action;
}

void Vim::addBindings(const QMap<QString, QString> &map)
{
    for (auto it = map.begin(); it != map.end(); ++it)
        actionMap.insert(it.key(), it.value());
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
