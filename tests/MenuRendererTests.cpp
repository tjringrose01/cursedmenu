#include <ctime>
#include <iostream>
#include <regex>

#include "MenuRenderer.hpp"

#define REQUIRE(condition)                                                     \
    do {                                                                       \
        if (!(condition)) {                                                    \
            std::cerr << "FAILED: " #condition << std::endl;                  \
            return 1;                                                          \
        }                                                                      \
    } while (false)

int main() {
    const std::time_t fixed = 1735689600; // 2025-01-01 00:00:00 UTC
    const cursedmenu::MenuRenderer::DateTimeParts parts =
        cursedmenu::MenuRenderer::formatLocalDateTime(fixed);

    const std::regex datePattern(R"(^\d{4}-\d{2}-\d{2}$)");
    const std::regex timePattern(R"(^\d{2}:\d{2}:\d{2} (AM|PM)$)");

    REQUIRE(std::regex_match(parts.date, datePattern));
    REQUIRE(std::regex_match(parts.time, timePattern));

    std::cout << "All MenuRenderer tests passed." << std::endl;
    return 0;
}
