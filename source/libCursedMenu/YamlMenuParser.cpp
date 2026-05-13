#include "YamlMenuParser.hpp"

#include <set>
#include <string>

#include <yaml-cpp/yaml.h>

#include "CursedMenuExceptions.hpp"
#include "MenuValidator.hpp"

namespace cursedmenu {

namespace {

constexpr std::size_t kMaxMenuFieldLength = 4096;

void addError(
    MenuParseResult& result,
    const std::filesystem::path& path,
    const std::string& location,
    const std::string& message) {
    result.errors.push_back({
        path.string(),
        location,
        message
    });
}

bool validateStringLength(
    MenuParseResult& result,
    const std::filesystem::path& path,
    const std::string& location,
    const std::string& value) {
    if (value.length() <= kMaxMenuFieldLength) {
        return true;
    }
    addError(
        result,
        path,
        location,
        "String value exceeds maximum length of "
            + std::to_string(kMaxMenuFieldLength)
            + " characters");
    return false;
}

bool hasOnlyAllowedKeys(
    const YAML::Node& node,
    const std::set<std::string>& allowedKeys,
    std::string& unknownKey) {
    if (!node || !node.IsMap()) {
        return true;
    }

    for (const auto& entry : node) {
        if (!entry.first.IsScalar()) {
            unknownKey = "<non-scalar-key>";
            return false;
        }
        const std::string key = entry.first.as<std::string>();
        if (allowedKeys.find(key) == allowedKeys.end()) {
            unknownKey = key;
            return false;
        }
    }
    return true;
}

[[noreturn]] void throwParserErrors(const MenuParseResult& result) {
    std::string message = "YAML parser failure";
    if (!result.errors.empty()) {
        message += ": ";
        for (std::size_t index = 0; index < result.errors.size(); ++index) {
            const auto& error = result.errors[index];
            message += "[" + error.location + "] " + error.message;
            if (index + 1 < result.errors.size()) {
                message += "; ";
            }
        }
    }
    throw ParserException(message);
}

[[noreturn]] void throwValidationErrors(
    const std::vector<MenuParseError>& errors) {
    std::string message = "Menu validation failure";
    if (!errors.empty()) {
        message += ": ";
        for (std::size_t index = 0; index < errors.size(); ++index) {
            const auto& error = errors[index];
            message += "[" + error.location + "] " + error.message;
            if (index + 1 < errors.size()) {
                message += "; ";
            }
        }
    }
    throw ValidationException(message);
}

} // namespace

bool YamlMenuParser::supportsFile(const std::filesystem::path& path) const noexcept {
    return path.extension() == ".yaml"
        || path.extension() == ".yml";
}

MenuParseResult YamlMenuParser::parseFile(const std::filesystem::path& path) const {
    MenuParseResult result;
    YAML::Node root;

    try {
        root = YAML::LoadFile(path.string());
    } catch (const YAML::Exception& exception) {
        addError(
            result,
            path,
            "yaml",
            std::string("Failed to parse YAML: ") + exception.what());
        throwParserErrors(result);
    }

    if (!root || !root.IsMap()) {
        addError(
            result,
            path,
            "root",
            "Top-level YAML element must be a map");
        throwParserErrors(result);
    }

    {
        std::string unknownKey;
        if (!hasOnlyAllowedKeys(
                root,
                {"version", "rootMenu", "settings", "menus"},
                unknownKey)) {
            addError(result, path, "root", "Unknown field: " + unknownKey);
        }
    }

    if (!root["version"] || !root["version"].IsScalar()) {
        addError(result, path, "version", "Missing or invalid integer version field");
    } else {
        try {
            result.menuDefinition.version = root["version"].as<int>();
        } catch (const YAML::Exception&) {
            addError(result, path, "version", "Missing or invalid integer version field");
        }
    }

    if (!root["rootMenu"] || !root["rootMenu"].IsScalar()) {
        addError(result, path, "rootMenu", "Missing or invalid rootMenu field");
    } else {
        result.menuDefinition.rootMenu = root["rootMenu"].as<std::string>();
        validateStringLength(result, path, "rootMenu", result.menuDefinition.rootMenu);
    }

    if (root["settings"]) {
        if (!root["settings"].IsMap()) {
            addError(result, path, "settings", "settings must be a map when present");
        } else {
            std::string unknownKey;
            if (!hasOnlyAllowedKeys(
                    root["settings"],
                    {"debug", "pauseAfterExecution"},
                    unknownKey)) {
                addError(result, path, "settings", "Unknown field: " + unknownKey);
            }

            if (root["settings"]["debug"]) {
                try {
                    result.menuDefinition.debug = root["settings"]["debug"].as<bool>();
                } catch (const YAML::Exception&) {
                    addError(result, path, "settings.debug", "settings.debug must be a boolean");
                }
            }
            if (root["settings"]["pauseAfterExecution"]) {
                try {
                    result.menuDefinition.pauseAfterExecution =
                        root["settings"]["pauseAfterExecution"].as<bool>();
                } catch (const YAML::Exception&) {
                    addError(
                        result,
                        path,
                        "settings.pauseAfterExecution",
                        "settings.pauseAfterExecution must be a boolean");
                }
            }
        }
    }

    if (!root["menus"] || !root["menus"].IsSequence()) {
        addError(result, path, "menus", "Missing or invalid menus array");
        throwParserErrors(result);
    }

    for (std::size_t menuIndex = 0; menuIndex < root["menus"].size(); ++menuIndex) {
        const YAML::Node menuNode = root["menus"][menuIndex];
        if (!menuNode.IsMap()) {
            addError(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "]",
                "Menu entry must be a map");
            continue;
        }

        {
            std::string unknownKey;
            if (!hasOnlyAllowedKeys(
                    menuNode,
                    {"id", "title", "foreground", "background", "items"},
                    unknownKey)) {
                addError(
                    result,
                    path,
                    "menus[" + std::to_string(menuIndex) + "]",
                    "Unknown field: " + unknownKey);
            }
        }

        Menu menu;
        if (!menuNode["id"] || !menuNode["id"].IsScalar()) {
            addError(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].id",
                "Menu id is required");
            continue;
        }

        menu.id = menuNode["id"].as<std::string>();
        if (!validateStringLength(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].id",
                menu.id)) {
            continue;
        }

        if (menuNode["title"] && menuNode["title"].IsScalar()) {
            menu.title = menuNode["title"].as<std::string>();
            validateStringLength(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].title",
                menu.title);
        }
        if (menuNode["foreground"] && menuNode["foreground"].IsScalar()) {
            menu.foreground = menuNode["foreground"].as<std::string>();
            validateStringLength(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].foreground",
                menu.foreground);
        }
        if (menuNode["background"] && menuNode["background"].IsScalar()) {
            menu.background = menuNode["background"].as<std::string>();
            validateStringLength(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].background",
                menu.background);
        }

        if (!menuNode["items"] || !menuNode["items"].IsSequence()) {
            addError(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].items",
                "Menu items array is required");
            continue;
        }

        for (std::size_t itemIndex = 0; itemIndex < menuNode["items"].size(); ++itemIndex) {
            const YAML::Node itemNode = menuNode["items"][itemIndex];
            if (!itemNode.IsMap()) {
                addError(
                    result,
                    path,
                    "menus[" + std::to_string(menuIndex)
                        + "].items[" + std::to_string(itemIndex) + "]",
                    "Menu item must be a map");
                continue;
            }

            {
                std::string unknownKey;
                if (!hasOnlyAllowedKeys(
                        itemNode,
                        {"name", "description", "command", "submenu", "action"},
                        unknownKey)) {
                    addError(
                        result,
                        path,
                        "menus[" + std::to_string(menuIndex)
                            + "].items[" + std::to_string(itemIndex) + "]",
                        "Unknown field: " + unknownKey);
                }
            }

            MenuItem item;
            if (!itemNode["name"] || !itemNode["name"].IsScalar()) {
                addError(
                    result,
                    path,
                    "menus[" + std::to_string(menuIndex)
                        + "].items[" + std::to_string(itemIndex) + "].name",
                    "Menu item name is required");
                continue;
            }

            item.name = itemNode["name"].as<std::string>();
            if (!validateStringLength(
                    result,
                    path,
                    "menus[" + std::to_string(menuIndex)
                        + "].items[" + std::to_string(itemIndex) + "].name",
                    item.name)) {
                continue;
            }

            if (itemNode["description"] && itemNode["description"].IsScalar()) {
                item.description = itemNode["description"].as<std::string>();
                validateStringLength(
                    result,
                    path,
                    "menus[" + std::to_string(menuIndex)
                        + "].items[" + std::to_string(itemIndex) + "].description",
                    item.description);
            }

            int actionCount = 0;
            if (itemNode["command"] && itemNode["command"].IsScalar()) {
                item.action.type = MenuActionType::Command;
                item.action.value = itemNode["command"].as<std::string>();
                validateStringLength(
                    result,
                    path,
                    "menus[" + std::to_string(menuIndex)
                        + "].items[" + std::to_string(itemIndex) + "].command",
                    item.action.value);
                ++actionCount;
            }
            if (itemNode["submenu"] && itemNode["submenu"].IsScalar()) {
                item.action.type = MenuActionType::Submenu;
                item.action.value = itemNode["submenu"].as<std::string>();
                validateStringLength(
                    result,
                    path,
                    "menus[" + std::to_string(menuIndex)
                        + "].items[" + std::to_string(itemIndex) + "].submenu",
                    item.action.value);
                ++actionCount;
            }
            if (itemNode["action"] && itemNode["action"].IsScalar()) {
                const std::string actionValue = itemNode["action"].as<std::string>();
                validateStringLength(
                    result,
                    path,
                    "menus[" + std::to_string(menuIndex)
                        + "].items[" + std::to_string(itemIndex) + "].action",
                    actionValue);
                if (actionValue == "exit") {
                    item.action.type = MenuActionType::Exit;
                    item.action.value = actionValue;
                    ++actionCount;
                } else {
                    addError(
                        result,
                        path,
                        "menus[" + std::to_string(menuIndex)
                            + "].items[" + std::to_string(itemIndex) + "].action",
                        "Unsupported action value: " + actionValue);
                }
            }

            if (actionCount != 1) {
                addError(
                    result,
                    path,
                    "menus[" + std::to_string(menuIndex)
                        + "].items[" + std::to_string(itemIndex) + "]",
                    "Menu item must define exactly one of command, submenu, or action");
                continue;
            }

            menu.items.push_back(item);
        }

        result.menuDefinition.menus.push_back(menu);
    }

    MenuValidator validator;
    auto validationErrors = validator.validate(
        result.menuDefinition,
        path.string());
    result.errors.insert(
        result.errors.end(),
        validationErrors.begin(),
        validationErrors.end());
    result.success = result.errors.empty();

    if (!validationErrors.empty()) {
        throwValidationErrors(validationErrors);
    }
    if (!result.success) {
        throwParserErrors(result);
    }
    return result;
}

} // namespace cursedmenu
