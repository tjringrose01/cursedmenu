cmake_minimum_required(VERSION 3.5)

project(cursedmenu
    VERSION 2.0.0
    DESCRIPTION "Cursed Menu aims to create an ncurses based menu system for character terminal sessions. This menu program could be used to create user, system administration, or utility menus for clients connecting with text based clients such as telnet, ssh, or rlogin"
    LANGUAGES CXX
)

find_package(Curses REQUIRED)

include_directories( ${CURSES_INCLUDE_DIR} ./source/libCursedMenu )

add_subdirectory( source )
