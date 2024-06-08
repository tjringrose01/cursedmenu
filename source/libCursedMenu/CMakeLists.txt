add_library(CursedMenu SHARED
    ActionLogger.cpp
    CursedMenu.cpp
    CursedMenuItem.cpp
    CursedMenuLoader.cpp
    debug.cpp
)

target_include_directories(CursedMenu PRIVATE
    ../include
)

target_link_libraries(CursedMenu PUBLIC
    Curses::Curses
)