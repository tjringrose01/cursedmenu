#include "MenuFileResolver.hpp"

#include <filesystem>

namespace cursedmenu {

std::string resolveMenuFilePath(const std::string& requestedMenuFile) {
    namespace fs = std::filesystem;

    if (requestedMenuFile.empty()) {
        return requestedMenuFile;
    }

    const fs::path directPath(requestedMenuFile);
    if (fs::exists(directPath)) {
        return requestedMenuFile;
    }

    if (directPath.has_parent_path()) {
        return requestedMenuFile;
    }

    const fs::path sourceMenuPath =
        fs::path("source") / "cursedmenu" / requestedMenuFile;
    if (fs::exists(sourceMenuPath)) {
        return sourceMenuPath.string();
    }

    const fs::path examplesMenuPath =
        fs::path("examples") / requestedMenuFile;
    if (fs::exists(examplesMenuPath)) {
        return examplesMenuPath.string();
    }

    return requestedMenuFile;
}

} // namespace cursedmenu
