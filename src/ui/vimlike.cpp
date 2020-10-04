#include "vimlike.h"

#include "mainwindow.h"

// TODO deprecated for now

#include <QAction>
#include <QKeySequence>

static bool vimlikeEnabled = false;

void enable(MainWindow &window)
{
    if (vimlikeEnabled) return;
    vimlikeEnabled = true;
    for (auto vimlikeBinding : vimlikeBindings.toStdMap())
    { // Iterate over all the defined vimlike bindings
        const auto &actions = window.actions(); // Make an alias
        for (int i = 0; i < window.actions().size(); ++i)
        { // Iterate over all actions in the window
            if (actions[i]->text() == vimlikeBinding.first)
            { // The text of the action matches the one in the binding list
                // The shortcut that is defined in non-vim-like mode
                auto existingShortcut = actions[i]->shortcuts();
                // Append the vimlike keysequence to the normal one
                existingShortcut.push_back(vimlikeBinding.second);
            }
        }
    }
}
