#include "MenuValidator.hpp"

#include <algorithm>
#include <cctype>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace cursedmenu {

namespace {
constexpr std::size_t kMaxSubmenuDepth = 32;

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

bool containsUnsafeControlCharacter(const std::string& value) {
    return std::any_of(
        value.begin(),
        value.end(),
        [](unsigned char character) {
            return character == '\n'
                || character == '\r'
                || character == '\0';
        });
}

std::size_t computeMaxDepth(
    const std::string& menuId,
    const std::unordered_map<std::string, std::set<std::string>>& graph,
    std::unordered_set<std::string>& recursionPath) {
    if (recursionPath.find(menuId) != recursionPath.end()) {
        // Navigation cycles are allowed; stop depth expansion at cycle edge.
        return 0;
    }

    recursionPath.insert(menuId);

    std::size_t maxChildDepth = 0;
    const auto graphIterator = graph.find(menuId);
    if (graphIterator != graph.end()) {
        for (const auto& childMenu : graphIterator->second) {
            maxChildDepth = std::max(
                maxChildDepth,
                computeMaxDepth(
                    childMenu,
                    graph,
                    recursionPath));
        }
    }

    recursionPath.erase(menuId);
    return maxChildDepth + 1;
}

} // namespace

std::vector<MenuParseError> MenuValidator::validate(
    const MenuDefinition& menuDefinition,
    const std::string& fileName) const {
    std::vector<MenuParseError> errors;

    std::unordered_map<std::string, const Menu*> menuMap;
    std::unordered_map<std::string, std::set<std::string>> submenuGraph;

    menuMap.reserve(menuDefinition.menus.size());
    submenuGraph.reserve(menuDefinition.menus.size());

    for (std::size_t menuIndex = 0;
         menuIndex < menuDefinition.menus.size();
         ++menuIndex) {
        const auto& menu = menuDefinition.menus[menuIndex];
        std::unordered_set<std::string> itemNames;
        itemNames.reserve(menu.items.size());

        if (menu.id.empty()) {
            addError(
                errors,
                fileName,
                "menus[" + std::to_string(menuIndex) + "].id",
                "Menu id cannot be empty");

            continue;
        }

        if (menuMap.find(menu.id) != menuMap.end()) {
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

        if (menu.items.empty()) {
            addError(
                errors,
                fileName,
                "menus[" + std::to_string(menuIndex) + "].items",
                "Menu must contain at least one item");
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

            if (itemNames.find(item.name) != itemNames.end()) {
                addError(
                    errors,
                    fileName,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "].name",
                    "Duplicate menu item name within menu: " + item.name);
            }
            itemNames.insert(item.name);

            if (containsUnsafeControlCharacter(item.action.value)) {
                addError(
                    errors,
                    fileName,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "].action",
                    "Action value contains unsafe control characters");
            }

            if (item.action.type == MenuActionType::Submenu) {
                if (item.action.value.empty()) {
                    addError(
                        errors,
                        fileName,
                        "menus["
                            + std::to_string(menuIndex)
                            + "].items["
                            + std::to_string(itemIndex)
                            + "].submenu",
                        "Submenu target cannot be empty");
                }
                submenuGraph[menu.id].insert(item.action.value);
            } else if (item.action.type == MenuActionType::Command) {
                if (item.action.value.empty()) {
                    addError(
                        errors,
                        fileName,
                        "menus["
                            + std::to_string(menuIndex)
                            + "].items["
                            + std::to_string(itemIndex)
                            + "].command",
                        "Command action cannot be empty");
                }
            } else if (item.action.type == MenuActionType::Exit
                       && item.action.value != "exit") {
                addError(
                    errors,
                    fileName,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "].action",
                    "Exit action must use value 'exit'");
            }
        }
    }

    if (menuDefinition.rootMenu.empty()) {
        addError(
            errors,
            fileName,
            "rootMenu",
            "Root menu cannot be empty");
    } else if (menuMap.find(menuDefinition.rootMenu) == menuMap.end()) {
        addError(
            errors,
            fileName,
            "rootMenu",
            "Root menu does not exist: "
                + menuDefinition.rootMenu);
    }

    for (const auto& [menuId, submenuTargets] : submenuGraph) {
        (void)menuId;

        for (const auto& submenuTarget : submenuTargets) {
            if (menuMap.find(submenuTarget) == menuMap.end()) {
                addError(
                    errors,
                    fileName,
                    "submenu",
                    "Submenu target does not exist: "
                        + submenuTarget);
            }
        }
    }

    for (const auto& [menuId, menuPointer] : menuMap) {
        (void)menuPointer;
        std::unordered_set<std::string> recursionPath;
        recursionPath.reserve(menuMap.size());
        const std::size_t depth = computeMaxDepth(
            menuId,
            submenuGraph,
            recursionPath);
        if (depth > kMaxSubmenuDepth) {
            addError(
                errors,
                fileName,
                "submenuGraph",
                "Submenu depth exceeds maximum of "
                    + std::to_string(kMaxSubmenuDepth)
                    + " from menu: "
                    + menuId);
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

    return validColors.find(colorName) != validColors.end();
}

} // namespace cursedmenu
