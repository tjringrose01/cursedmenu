#ifndef CURSEDMENU_CURSED_MENU_RUNNER_HPP
#define CURSEDMENU_CURSED_MENU_RUNNER_HPP

#include <stack>

#include "ActionLogger.hpp"
#include "CursedMenu.hpp"

namespace cursedmenu {

/**
 * Owns the interactive ncurses menu runtime.
 *
 * This class is an incremental modernization step that moves terminal runtime
 * behavior out of main.cpp and into libCursedMenu. Future work should continue
 * decomposing this class into smaller terminal, rendering, and command execution
 * components.
 */
class CursedMenuRunner {
public:
    explicit CursedMenuRunner(ActionLogger& actionLogger);

    /**
     * Runs the interactive menu stack until the user exits.
     */
    void run(std::stack<CursedMenu>& menus);

private:
    ActionLogger& actionLogger;
};

} // namespace cursedmenu

#endif // CURSEDMENU_CURSED_MENU_RUNNER_HPP
