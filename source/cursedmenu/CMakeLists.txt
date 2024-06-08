add_executable( cursedmenu main.cpp )
target_link_libraries( cursedmenu PUBLIC CursedMenu )
target_include_directories( cursedmenu PUBLIC ../libCursedmenu )
