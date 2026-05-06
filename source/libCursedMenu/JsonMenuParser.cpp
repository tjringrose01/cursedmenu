#include "JsonMenuParser.hpp"

#include <fstream>
#include <sstream>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

#include "MenuValidator.hpp"

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
    const rapidjson::Value& jsonObject,
    const char* fieldName) {
    return jsonObject.HasMember(fieldName)
        && jsonObject[fieldName].IsString();
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

    std::stringstream buffer;
    buffer << inputFile.rdbuf();

    rapidjson::Document document;

    document.Parse(buffer.str().c_str());

    if (document.HasParseError()) {
        addError(
            result,
            path,
            "json",
            std::string("Failed to parse JSON: ")
                + rapidjson::GetParseError_En(document.GetParseError()));

        return result;
    }

    if (!document.IsObject()) {
        addError(
            result,
            path,
            "root",
            "Top-level JSON element must be an object");

        return result;
    }

    if (!document.HasMember("version")
        || !document["version"].IsInt()) {
        addError(
            result,
            path,
            "version",
            "Missing or invalid integer version field");
    } else {
        result.menuDefinition.version = document["version"].GetInt();
    }

    if (!hasStringField(document, "rootMenu")) {
        addError(
            result,
            path,
            "rootMenu",
            "Missing or invalid rootMenu field");
    } else {
        result.menuDefinition.rootMenu =
            document["rootMenu"].GetString();
    }

    if (document.HasMember("settings")
        && document["settings"].IsObject()) {
        const auto& settings = document["settings"];

        if (settings.HasMember("debug")
            && settings["debug"].IsBool()) {
            result.menuDefinition.debug = settings["debug"].GetBool();
        }

        if (settings.HasMember("pauseAfterExecution")
            && settings["pauseAfterExecution"].IsBool()) {
            result.menuDefinition.pauseAfterExecution =
                settings["pauseAfterExecution"].GetBool();
        }
    }

    if (!document.HasMember("menus")
        || !document["menus"].IsArray()) {
        addError(
            result,
            path,
            "menus",
            "Missing or invalid menus array");

        return result;
    }

    const auto& menusJson = document["menus"];

    for (rapidjson::SizeType menuIndex = 0;
         menuIndex < menusJson.Size();
         ++menuIndex) {
        const auto& menuJson = menusJson[menuIndex];

        if (!menuJson.IsObject()) {
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

        menu.id = menuJson["id"].GetString();

        if (hasStringField(menuJson, "title")) {
            menu.title = menuJson["title"].GetString();
        }

        if (hasStringField(menuJson, "foreground")) {
            menu.foreground = menuJson["foreground"].GetString();
        }

        if (hasStringField(menuJson, "background")) {
            menu.background = menuJson["background"].GetString();
        }

        if (!menuJson.HasMember("items")
            || !menuJson["items"].IsArray()) {
            addError(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].items",
                "Menu items array is required");

            continue;
        }

        const auto& itemsJson = menuJson["items"];

        for (rapidjson::SizeType itemIndex = 0;
             itemIndex < itemsJson.Size();
             ++itemIndex) {
            const auto& itemJson = itemsJson[itemIndex];

            if (!itemJson.IsObject()) {
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

            item.name = itemJson["name"].GetString();

            if (hasStringField(itemJson, "description")) {
                item.description = itemJson["description"].GetString();
            }

            int actionCount = 0;

            if (hasStringField(itemJson, "command")) {
                item.action.type = MenuActionType::Command;
                item.action.value = itemJson["command"].GetString();
                ++actionCount;
            }

            if (hasStringField(itemJson, "submenu")) {
                item.action.type = MenuActionType::Submenu;
                item.action.value = itemJson["submenu"].GetString();
                ++actionCount;
            }

            if (hasStringField(itemJson, "action")) {
                const std::string actionValue =
                    itemJson["action"].GetString();

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

    MenuValidator validator;

    auto validationErrors = validator.validate(
        result.menuDefinition,
        path.string());

    result.errors.insert(
        result.errors.end(),
        validationErrors.begin(),
        validationErrors.end());

    result.success = result.errors.empty();

    return result;
}

} // namespace cursedmenu
