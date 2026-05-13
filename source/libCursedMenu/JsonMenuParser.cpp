#include "JsonMenuParser.hpp"

#include <fstream>
#include <set>
#include <sstream>
#include <unordered_set>

#include <rapidjson/document.h>
#include <rapidjson/error/en.h>

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

bool hasStringField(
    const rapidjson::Value& jsonObject,
    const char* fieldName) {
    return jsonObject.HasMember(fieldName)
        && jsonObject[fieldName].IsString();
}

void validateAllowedKeys(
    MenuParseResult& result,
    const std::filesystem::path& path,
    const rapidjson::Value& object,
    const std::string& location,
    const std::set<std::string>& allowedKeys) {
    for (auto memberIterator = object.MemberBegin();
         memberIterator != object.MemberEnd();
         ++memberIterator) {
        const std::string key = memberIterator->name.GetString();
        if (allowedKeys.find(key) == allowedKeys.end()) {
            addError(
                result,
                path,
                location,
                "Unknown field: " + key);
        }
    }
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

bool findDuplicateKeyPath(
    const rapidjson::Value& value,
    const std::string& currentPath,
    std::string& duplicateKeyPath) {
    if (value.IsObject()) {
        std::unordered_set<std::string> keys;
        keys.reserve(value.MemberCount());

        for (auto memberIterator = value.MemberBegin();
             memberIterator != value.MemberEnd();
             ++memberIterator) {
            const std::string key = memberIterator->name.GetString();
            if (keys.find(key) != keys.end()) {
                duplicateKeyPath = currentPath.empty()
                    ? key
                    : currentPath + "." + key;
                return true;
            }
            keys.insert(key);

            const std::string childPath = currentPath.empty()
                ? key
                : currentPath + "." + key;
            if (findDuplicateKeyPath(
                    memberIterator->value,
                    childPath,
                    duplicateKeyPath)) {
                return true;
            }
        }
    } else if (value.IsArray()) {
        for (rapidjson::SizeType index = 0; index < value.Size(); ++index) {
            const std::string childPath =
                currentPath + "[" + std::to_string(index) + "]";
            if (findDuplicateKeyPath(
                    value[index],
                    childPath,
                    duplicateKeyPath)) {
                return true;
            }
        }
    }

    return false;
}

[[noreturn]] void throwParserErrors(const MenuParseResult& result) {
    std::string message = "JSON parser failure";

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

[[noreturn]] void throwValidationErrors(const std::vector<MenuParseError>& errors) {
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

bool JsonMenuParser::supportsFile(
    const std::filesystem::path& path) const noexcept {
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
        throwParserErrors(result);
    }

    std::stringstream buffer;
    buffer << inputFile.rdbuf();

    rapidjson::Document document;

    document.Parse<rapidjson::kParseValidateEncodingFlag>(
        buffer.str().c_str());

    if (document.HasParseError()) {
        addError(
            result,
            path,
            "json",
            std::string("Failed to parse JSON: ")
                + rapidjson::GetParseError_En(document.GetParseError()));

        throwParserErrors(result);
    }

    if (!document.IsObject()) {
        addError(
            result,
            path,
            "root",
            "Top-level JSON element must be an object");

        throwParserErrors(result);
    }

    std::string duplicateKeyPath;
    if (findDuplicateKeyPath(document, "", duplicateKeyPath)) {
        addError(
            result,
            path,
            duplicateKeyPath.empty() ? "json" : duplicateKeyPath,
            "Duplicate JSON object key detected");
        throwParserErrors(result);
    }

    validateAllowedKeys(
        result,
        path,
        document,
        "root",
        {"version", "rootMenu", "settings", "menus"});

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
        validateStringLength(
            result,
            path,
            "rootMenu",
            result.menuDefinition.rootMenu);
    }

    if (document.HasMember("settings")
        && !document["settings"].IsObject()) {
        addError(
            result,
            path,
            "settings",
            "settings must be an object when present");
    } else if (document.HasMember("settings")
               && document["settings"].IsObject()) {
        const auto& settings = document["settings"];
        validateAllowedKeys(
            result,
            path,
            settings,
            "settings",
            {
                "debug",
                "debugMode",
                "pauseAfterExecution",
                "pauseAfterCommand",
                "pause_after_execution",
                "pause_after_command",
                "showDate",
                "displayDate",
                "show_date",
                "display_date",
                "showTime",
                "displayTime",
                "show_time",
                "display_time"
            });

        const auto parseBoolSetting =
            [&](const std::vector<std::string>& keys,
                bool& targetValue) {
                for (const auto& key : keys) {
                    if (!settings.HasMember(key.c_str())) {
                        continue;
                    }
                    if (!settings[key.c_str()].IsBool()) {
                        addError(
                            result,
                            path,
                            "settings." + key,
                            "settings." + key + " must be a boolean");
                        return;
                    }
                    targetValue = settings[key.c_str()].GetBool();
                    return;
                }
            };

        parseBoolSetting(
            {"debug", "debugMode"},
            result.menuDefinition.debug);
        parseBoolSetting(
            {
                "pauseAfterExecution",
                "pauseAfterCommand",
                "pause_after_execution",
                "pause_after_command"
            },
            result.menuDefinition.pauseAfterExecution);
        parseBoolSetting(
            {"showDate", "displayDate", "show_date", "display_date"},
            result.menuDefinition.showDate);
        parseBoolSetting(
            {"showTime", "displayTime", "show_time", "display_time"},
            result.menuDefinition.showTime);
    }

    if (!document.HasMember("menus")
        || !document["menus"].IsArray()) {
        addError(
            result,
            path,
            "menus",
            "Missing or invalid menus array");

        throwParserErrors(result);
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
        validateAllowedKeys(
            result,
            path,
            menuJson,
            "menus[" + std::to_string(menuIndex) + "]",
            {"id", "title", "foreground", "background", "items"});

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
        if (!validateStringLength(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].id",
                menu.id)) {
            continue;
        }

        if (hasStringField(menuJson, "title")) {
            menu.title = menuJson["title"].GetString();
            validateStringLength(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].title",
                menu.title);
        }

        if (hasStringField(menuJson, "foreground")) {
            menu.foreground = menuJson["foreground"].GetString();
            validateStringLength(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].foreground",
                menu.foreground);
        }

        if (hasStringField(menuJson, "background")) {
            menu.background = menuJson["background"].GetString();
            validateStringLength(
                result,
                path,
                "menus[" + std::to_string(menuIndex) + "].background",
                menu.background);
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
            validateAllowedKeys(
                result,
                path,
                itemJson,
                "menus["
                    + std::to_string(menuIndex)
                    + "].items["
                    + std::to_string(itemIndex)
                    + "]",
                {"name", "description", "command", "submenu", "action"});

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
            if (!validateStringLength(
                    result,
                    path,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "].name",
                    item.name)) {
                continue;
            }

            if (hasStringField(itemJson, "description")) {
                item.description = itemJson["description"].GetString();
                validateStringLength(
                    result,
                    path,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "].description",
                    item.description);
            }

            int actionCount = 0;

            if (hasStringField(itemJson, "command")) {
                item.action.type = MenuActionType::Command;
                item.action.value = itemJson["command"].GetString();
                validateStringLength(
                    result,
                    path,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "].command",
                    item.action.value);
                ++actionCount;
            }

            if (hasStringField(itemJson, "submenu")) {
                item.action.type = MenuActionType::Submenu;
                item.action.value = itemJson["submenu"].GetString();
                validateStringLength(
                    result,
                    path,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "].submenu",
                    item.action.value);
                ++actionCount;
            }

            if (hasStringField(itemJson, "action")) {
                const std::string actionValue =
                    itemJson["action"].GetString();
                validateStringLength(
                    result,
                    path,
                    "menus["
                        + std::to_string(menuIndex)
                        + "].items["
                        + std::to_string(itemIndex)
                        + "].action",
                    actionValue);

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

    if (!validationErrors.empty()) {
        throwValidationErrors(validationErrors);
    }

    if (!result.success) {
        throwParserErrors(result);
    }

    return result;
}

} // namespace cursedmenu
