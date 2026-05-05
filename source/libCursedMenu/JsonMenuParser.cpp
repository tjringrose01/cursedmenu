#include "JsonMenuParser.hpp"

#include <fstream>

#include <nlohmann/json.hpp>

namespace cursedmenu {

namespace {

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

bool hasStringField(
    const nlohmann::json& jsonObject,
    const char* fieldName) {
    return jsonObject.contains(fieldName)
        && jsonObject[fieldName].is_string();
}

} // namespace

bool JsonMenuParser::supportsFile(
    const std::filesystem::path& path) const {
    return path.extension() == ".json";
}

MenuParseResult JsonMenuParser::parseFile(
    const std::filesystem::path& path) const {
    MenuParseResult result;

    std::ifstream inputFile(path);

    if (!inputFile.is_open()) {
        addError(
            result,
            path,
            "file",
            "Unable to open JSON menu file");

        return result;
    }

    nlohmann::json rootJson;

    try {
        inputFile >> rootJson;
    } catch (const std::exception& exception) {
        addError(
            result,
            path,
            "json",
            std::string("Failed to parse JSON: ") + exception.what());

        return result;
    }

    if (!rootJson.is_object()) {
        addError(
            result,
            path,
            "root",
            "Top-level JSON element must be an object");

        return result;
    }

    if (!rootJson.contains("version")
        || !rootJson["version"].is_number_integer()) {
        addError(
            result,
            path,
            "version",
            "Missing or invalid integer version field");
    } else {
        result.menuDefinition.version = rootJson["version"];
    }

    if (!hasStringField(rootJson, "rootMenu")) {
        addError(
            result,
            path,
            "rootMenu",
            "Missing or invalid rootMenu field");
    } else {
        result.menuDefinition.rootMenu = rootJson["rootMenu"];
    }

    if (rootJson.contains("settings")
        && rootJson["settings"].is_object()) {
        const auto& settings = rootJson["settings"];

        if (settings.contains("debug")
            && settings["debug"].is_boolean()) {
            result.menuDefinition.debug = settings["debug"];
        }

        if (settings.contains("pauseAfterExecution")
            && settings["pauseAfterExecution"].is_boolean()) {
            result.menuDefinition.pauseAfterExecution =
                settings["pauseAfterExecution"];
        }
    }

    if (!rootJson.contains("menus")
        || !rootJson["menus"].is_array()) {
        addError(
            result,
            path,
            "menus",
            "Missing or invalid menus array");

        return result;
    }

    const auto& menusJson = rootJson["menus"];

    for (std::size_t menuIndex = 0;
         menuIndex < menusJson.size();
         ++menuIndex) {
        const auto& menuJson = menusJson[menuIndex];

        if (!menuJson.is_object()) {
            addError(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "]",
                "Menu entry must be an object");

            continue;
        }

        Menu menu;

        if (!hasStringField(menuJson, "id")) {
            addError(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].id",
                "Menu id is required");

            continue;
        }

        menu.id = menuJson["id"];

        if (hasStringField(menuJson, "title")) {
            menu.title = menuJson["title"];
        }

        if (hasStringField(menuJson, "foreground")) {
            menu.foreground = menuJson["foreground"];
        }

        if (hasStringField(menuJson, "background")) {
            menu.background = menuJson["background"];
        }

        if (!menuJson.contains("items")
            || !menuJson["items"].is_array()) {
            addError(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].items",
                "Menu items array is required");

            continue;
        }

        const auto& itemsJson = menuJson["items"];

        for (std::size_t itemIndex = 0;
             itemIndex < itemsJson.size();
             ++itemIndex) {
            const auto& itemJson = itemsJson[itemIndex];

            if (!itemJson.is_object()) {
                addError(
                    result,
                    path,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "]",
                    "Menu item must be an object");

                continue;
            }

            MenuItem item;

            if (!hasStringField(itemJson, "name")) {
                addError(
                    result,
                    path,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "].name",
                    "Menu item name is required");

                continue;
            }

            item.name = itemJson["name"];

            if (hasStringField(itemJson, "description")) {
                item.description = itemJson["description"];
            }

            int actionCount = 0;

            if (hasStringField(itemJson, "command")) {
                item.action.type = MenuActionType::Command;
                item.action.value = itemJson["command"];
                ++actionCount;
            }

            if (hasStringField(itemJson, "submenu")) {
                item.action.type = MenuActionType::Submenu;
                item.action.value = itemJson["submenu"];
                ++actionCount;
            }

            if (hasStringField(itemJson, "action")) {
                const std::string actionValue = itemJson["action"];

                if (actionValue == "exit") {
                    item.action.type = MenuActionType::Exit;
                    item.action.value = actionValue;
                    ++actionCount;
                } else {
                    addError(
                        result,
                        path,
                        "menus["
                            + std::to_string(menuIndex)
                            + "].items["
                            + std::to_string(itemIndex)
                            + "].action",
                        "Unsupported action value: " + actionValue);
                }
            }

            if (actionCount != 1) {
                addError(
                    result,
                    path,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "]",
                    "Menu item must define exactly one of command, submenu, or action");

                continue;
            }

            menu.items.push_back(item);
        }

        result.menuDefinition.menus.push_back(menu);
    }

    result.success = result.errors.empty();

    return result;
}

} // namespace cursedmenu
