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
