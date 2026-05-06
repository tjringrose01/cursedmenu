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
)
