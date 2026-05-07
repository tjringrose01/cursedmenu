#include "RuntimeAction.hpp"

namespace cursedmenu {

namespace {

std::string trimLeadingSpaces(std::string value) {
    const auto firstNonSpace = value.find_first_not_of(' ');

    if (firstNonSpace == std::string::npos) {
        return "";
    }

    return value.substr(firstNonSpace);
}

} // namespace

RuntimeAction classifyRuntimeAction(const std::string& execText) {
    if (execText == "MenuExit") {
        return {RuntimeActionType::ExitMenu, ""};
    }

    static constexpr const char* submenuPrefix = "MenuSub ";

    if (execText.find(submenuPrefix) == 0) {
        return {
            RuntimeActionType::OpenSubmenu,
            trimLeadingSpaces(execText.substr(8))
        };
    }

    return {RuntimeActionType::ExecuteCommand, execText};
}

} // namespace cursedmenu
