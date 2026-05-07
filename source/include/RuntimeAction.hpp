#ifndef CURSEDMENU_RUNTIME_ACTION_HPP
#define CURSEDMENU_RUNTIME_ACTION_HPP

#include <string>

namespace cursedmenu {

enum class RuntimeActionType {
    ExitMenu,
    OpenSubmenu,
    ExecuteCommand
};

struct RuntimeAction {
    RuntimeActionType type;
    std::string value;
};

RuntimeAction classifyRuntimeAction(const std::string& execText);

} // namespace cursedmenu

#endif // CURSEDMENU_RUNTIME_ACTION_HPP
