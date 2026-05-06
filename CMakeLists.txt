cmake_minimum_required(VERSION 3.25)

project(cursedmenu
    VERSION 2.0.0
    DESCRIPTION "Cursed Menu aims to create an ncurses based menu system for character terminal sessions. This menu program could be used to create user, system administration, or utility menus for clients connecting with text based clients such as telnet, ssh, or rlogin"
    LANGUAGES CXX
)

include(CTest)

find_package(Curses REQUIRED)
find_package(nlohmann_json REQUIRED)

include_directories( ${CURSES_INCLUDE_DIR} ./source/libCursedMenu )

add_subdirectory( source )

if(BUILD_TESTING)
    add_subdirectory( tests )
endif()
