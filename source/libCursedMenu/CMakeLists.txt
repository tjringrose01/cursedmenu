add_library(CursedMenu SHARED
    ActionLogger.cpp
    CursedMenu.cpp
    CursedMenuItem.cpp
    CursedMenuLoader.cpp
    CursedMenuRunner.cpp
    JsonMenuParser.cpp
    LegacyCmdMenuParser.cpp
    MenuRenderer.cpp
    NcursesMenu.cpp
    MenuParserFactory.cpp
    MenuValidator.cpp
    NcursesSession.cpp
    NcursesWindow.cpp
    RuntimeAction.cpp
    YamlMenuParser.cpp
    debug.cpp
)

target_include_directories(CursedMenu
    PRIVATE
        ../include

    SYSTEM PRIVATE
        ${CONAN_INCLUDE_DIRS_RAPIDJSON}
)

target_link_libraries(CursedMenu PUBLIC
    Curses::Curses
    yaml-cpp::yaml-cpp
)
