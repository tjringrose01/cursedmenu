#include "MenuValidator.hpp"

#include <set>
#include <unordered_map>
#include <unordered_set>

namespace cursedmenu {

namespace {

void addError(
    std::vector<MenuParseError>& errors,
    const std::string& fileName,
    const std::string& location,
    const std::string& message) {
    errors.push_back({
        fileName,
        location,
        message
    });
}

bool detectCycle(
    const std::string& menuId,
    const std::unordered_map<std::string, std::set<std::string>>& graph,
    std::unordered_set<std::string>& visited,
    std::unordered_set<std::string>& recursionStack) {
    if (recursionStack.contains(menuId)) {
        return true;
    }

    if (visited.contains(menuId)) {
        return false;
    }

    visited.insert(menuId);
    recursionStack.insert(menuId);

    auto graphIterator = graph.find(menuId);

    if (graphIterator != graph.end()) {
        for (const auto& childMenu : graphIterator->second) {
            if (detectCycle(childMenu, graph, visited, recursionStack)) {
                return true;
            }
        }
    }

    recursionStack.erase(menuId);

    return false;
}

} // namespace

std::vector<MenuParseError> MenuValidator::validate(
    const MenuDefinition& menuDefinition,
    const std::string& fileName) const {
    std::vector<MenuParseError> errors;

    std::unordered_map<std::string, const Menu*> menuMap;
    std::unordered_map<std::string, std::set<std::string>> submenuGraph;

    for (std::size_t menuIndex = 0;
         menuIndex < menuDefinition.menus.size();
         ++menuIndex) {
        const auto& menu = menuDefinition.menus[menuIndex];

        if (menu.id.empty()) {
            addError(
                errors,
                fileName,
                "menus[" + std::to_string(menuIndex) + "].id",
                "Menu id cannot be empty");

            continue;
        }

        if (menuMap.contains(menu.id)) {
            addError(
                errors,
                fileName,
                "menus[" + std::to_string(menuIndex) + "].id",
                "Duplicate menu id: " + menu.id);
        }

        menuMap[menu.id] = &menu;

        if (menu.title.empty()) {
            addError(
                errors,
                fileName,
                "menus[" + std::to_string(menuIndex) + "].title",
                "Menu title should not be empty");
        }

        if (!menu.foreground.empty()
            && !isKnownColor(menu.foreground)) {
            addError(
                errors,
                fileName,
                "menus[" + std::to_string(menuIndex) + "].foreground",
                "Unknown foreground color: " + menu.foreground);
        }

        if (!menu.background.empty()
            && !isKnownColor(menu.background)) {
            addError(
                errors,
                fileName,
                "menus[" + std::to_string(menuIndex) + "].background",
                "Unknown background color: " + menu.background);
        }

        for (std::size_t itemIndex = 0;
             itemIndex < menu.items.size();
             ++itemIndex) {
            const auto& item = menu.items[itemIndex];

            if (item.name.empty()) {
                addError(
                    errors,
                    fileName,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "].name",
                    "Menu item name cannot be empty");
            }

            if (item.action.type == MenuActionType::Submenu) {
                submenuGraph[menu.id].insert(item.action.value);
            }
        }
    }

    if (menuDefinition.rootMenu.empty()) {
        addError(
            errors,
            fileName,
            "rootMenu",
            "Root menu cannot be empty");
    } else if (!menuMap.contains(menuDefinition.rootMenu)) {
        addError(
            errors,
            fileName,
            "rootMenu",
            "Root menu does not exist: "
                + menuDefinition.rootMenu);
    }

    for (const auto& [menuId, submenuTargets] : submenuGraph) {
        for (const auto& submenuTarget : submenuTargets) {
            if (!menuMap.contains(submenuTarget)) {
                addError(
                    errors,
                    fileName,
                    "submenu",
                    "Submenu target does not exist: "
                        + submenuTarget);
            }
        }
    }

    std::unordered_set<std::string> visited;
    std::unordered_set<std::string> recursionStack;

    for (const auto& [menuId, menuPointer] : menuMap) {
        if (detectCycle(
                menuId,
                submenuGraph,
                visited,
                recursionStack)) {
            addError(
                errors,
                fileName,
                "submenuGraph",
                "Detected submenu cycle involving menu: " + menuId);

            break;
        }
    }

    return errors;
}

bool MenuValidator::isKnownColor(
    const std::string& colorName) const {
    static const std::set<std::string> validColors = {
        "BLACK",
        "RED",
        "GREEN",
        "YELLOW",
        "BLUE",
        "MAGENTA",
        "CYAN",
        "WHITE"
    };

    return validColors.contains(colorName);
}

} // namespace cursedmenu
