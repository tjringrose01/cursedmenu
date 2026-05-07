#include <iostream>

#include "RuntimeAction.hpp"

#define REQUIRE(condition)                                                     \
    do {                                                                       \
        if (!(condition)) {                                                    \
            std::cerr << "FAILED: " #condition << std::endl;                  \
            return 1;                                                          \
        }                                                                      \
    } while (false)

using namespace cursedmenu;

int main() {
    const RuntimeAction exitAction = classifyRuntimeAction("MenuExit");
    REQUIRE(exitAction.type == RuntimeActionType::ExitMenu);
    REQUIRE(exitAction.value.empty());

    const RuntimeAction submenuAction =
        classifyRuntimeAction("MenuSub sub.cmd");
    REQUIRE(submenuAction.type == RuntimeActionType::OpenSubmenu);
    REQUIRE(submenuAction.value == "sub.cmd");

    const RuntimeAction submenuActionSpaced =
        classifyRuntimeAction("MenuSub    nested/submenu.cmd");
    REQUIRE(submenuActionSpaced.type == RuntimeActionType::OpenSubmenu);
    REQUIRE(submenuActionSpaced.value == "nested/submenu.cmd");

    const RuntimeAction commandAction =
        classifyRuntimeAction("uname -a");
    REQUIRE(commandAction.type == RuntimeActionType::ExecuteCommand);
    REQUIRE(commandAction.value == "uname -a");

    std::cout << "All RuntimeAction tests passed." << std::endl;

    return 0;
}
