add_executable(JsonMenuParserTests
    JsonMenuParserTests.cpp
)

target_include_directories(JsonMenuParserTests PRIVATE
    ../source/include
)

target_link_libraries(JsonMenuParserTests PRIVATE
    CursedMenu
)

add_test(
    NAME JsonMenuParserTests
    COMMAND JsonMenuParserTests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(MenuParserFactoryTests
    MenuParserFactoryTests.cpp
)

target_include_directories(MenuParserFactoryTests PRIVATE
    ../source/include
)

target_link_libraries(MenuParserFactoryTests PRIVATE
    CursedMenu
)

add_test(
    NAME MenuParserFactoryTests
    COMMAND MenuParserFactoryTests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(RuntimeActionTests
    RuntimeActionTests.cpp
)

target_include_directories(RuntimeActionTests PRIVATE
    ../source/include
)

target_link_libraries(RuntimeActionTests PRIVATE
    CursedMenu
)

add_test(
    NAME RuntimeActionTests
    COMMAND RuntimeActionTests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(CursedMenuLoaderExceptionTests
    CursedMenuLoaderExceptionTests.cpp
)

target_include_directories(CursedMenuLoaderExceptionTests PRIVATE
    ../source/include
)

target_link_libraries(CursedMenuLoaderExceptionTests PRIVATE
    CursedMenu
)

add_test(
    NAME CursedMenuLoaderExceptionTests
    COMMAND CursedMenuLoaderExceptionTests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(CursedMenuExceptionTests
    CursedMenuExceptionTests.cpp
)

target_include_directories(CursedMenuExceptionTests PRIVATE
    ../source/include
)

target_link_libraries(CursedMenuExceptionTests PRIVATE
    CursedMenu
)

add_test(
    NAME CursedMenuExceptionTests
    COMMAND CursedMenuExceptionTests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

add_executable(MenuFileResolverTests
    MenuFileResolverTests.cpp
)

target_include_directories(MenuFileResolverTests PRIVATE
    ../source/include
)

target_link_libraries(MenuFileResolverTests PRIVATE
    CursedMenu
)

add_test(
    NAME MenuFileResolverTests
    COMMAND MenuFileResolverTests
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
