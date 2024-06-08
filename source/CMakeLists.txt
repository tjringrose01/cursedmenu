add_subdirectory( libCursedMenu )

add_executable( cursedmenu main.cpp )
target_link_libraries( cursedmenu PUBLIC libCursedMenu )
target_include_directories( cursedmenu PUBLIC ./libCursedmenu )