/// @file vim.h
#ifndef VIM_H
#define VIM_H

#include <QKeyEvent>

/**
  * Repeat a command `N` times.
  *
  * Usage:
  *
  * ```
  * 	vimdo(N) single_command
  * 	vimdo(N) { multiple_commands }
  * ```
  *
  * @note This is a convenience macro.
  */
#define vimdo(N) for (int Intentionally_Long_Unlikely_Name = 0;\
    Intentionally_Long_Unlikely_Name < N; ++Intentionally_Long_Unlikely_Name)

/**
 * @brief Everything related to vim-mode inside schim.
 */
namespace Vim
{

/**
 * @brief Representation of a vim-count.
 *
 * When the user inputs a key sequence, usually a count can also be specified.
 * This count determines how many times the command will be repeated. An unspecified
 * count is equal to `Count(0)`.
 *
 * Different commands will treat a 0-count differently. Some commands equate a
 * 0-count with a 1-count, others treat it in some special way.
 * This class provides a uniform interface for each
 * use case.
 *
 * Casting a `Vim::Count` to an `int` yields a *basic* count, i.e.
 * `int(Count(0)) == 1`. On the other hand, `raw()` yields the *raw* count, i.e.
 * `Count(0).raw() == 0`.
 *
 * @note For non-zero counts, `int()` and `raw()` are equivalent.
 *
 * @see `Vim::N`
 */
class Count
{
    int count;
public:
    /**
     * @brief Construct a vim-count.
     * @param count The raw vim-count.
     */
    Count(int count);

    /**
     * @brief Conversion to integer.
     * @returns The **basic** count.
     */
    operator int() const;
    /// @brief Get the raw count from this `Count`.
    int raw() const;
};

/// @brief Convenience typedef.
typedef Count N;

/**
 * @brief An action in vim-mode.
 *
 * A vim-action is a key sequence that is accompanied with some extra data:
 *
 * - a vim-count [optional]
 * - input register [TODO]
 * - a vim-command
 * - a vim-motion [TODO]
 * - output register [TODO]
 */
class Action
{
    // A count of 0 means that no count was specified
    int count = 0;
    // in_reg = -1 means that the stroke takes no input register
    char in_reg = -1;
    QString command;
    //TODO motion;

public:
    /// @brief Construct a vim-action.
    Action(const QString &command, int count = 0);

    /// @brief Get the vim-command associated with this action.
    QString getCommand() const;
    /// @brief Get the vim-count associated with this action.
    Count getCount() const;

    /// @brief Is this action's command identical to `cmd`?
    bool operator==(const QString &cmd) const;
};

/// @brief Enable/Disable vim-mode throughout the application instance.
void enable(bool enable);
/// @brief Is vim-mode enabled?
bool enabled();
/// @brief The vim-count from the current key sequence.
Count n();

/**
 * @brief Register a keybinding with Vim-mode.
 * @param sequence The key sequence that triggers `action`.
 * @param action The vim-action that is performed when the key sequence is input.
 */
void addBinding(const QString &sequence, const QString &action);
/**
 * @brief Convenient alternative to multiple calls of `Vim::addBinding`.
 * @param map Map of bindings to be registered with vim-mode.
 */
void addBindings(const QMap<QString, QString> &map);
/**
 * @brief Process a key press in vim-mode.
 * @param event KeyPress event that triggered this.
 * @param callback Callback for when the input key sequence matches a binding.
 * @param allowCount Process vim-counts as part of a key sequence.
 *
 * When a widget receives a KeyPress event, it should call this function if
 * vim-like behavior is desired. The event will be processed, the current vim
 * key sequence will be updated and it will be shown in the vim status indicator
 * of the main window. If the sequence matches a binding (see `Vim::addBinding`)
 * callback will be called with an appropriate `Vim::Action`.
 *
 * @note If vim-mode is not enabled, this function does nothing.
 * @note If `allowCount` is `false`, treat numbers as normal key sequences.
 *
 * @see `Vim::Action`
 *
 * ### Implementation details
 *
 * First we determine if we are listening for the user to input a count (only if
 * `allowCount` is `true`). We correspondingly process all digits the user
 * enters and form a count.
 *
 * Then, we test if the sequence matches a binding. If it does, `callback` is
 * called. Otherwise, we test if any binding starts with the sequence that has
 * been input so far. If even this is not the case, the sequence is discarded
 * and the vim status is reset.
 *
 */
void registerKeyPress(QKeyEvent *event,
                      std::function<bool(const Action &action)> callback,
                      bool allowCount = true);
/**
 * @brief Discard the current key sequence and reset vim status indicator.
 */
void resetStroke();

/// @brief Get the vim status text for the current key sequence.
QString getStatusText();

}

#endif // VIM_H
