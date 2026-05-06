# CursedMenu Coding Standards

## Purpose

These standards define how CursedMenu code should be written and reviewed during modernization. The goals are readability, maintainability, safety, consistency, and small reviewable changes.

These standards apply to new code and to existing code that is touched during modernization. Existing files do not need to be reformatted all at once unless a dedicated formatting-only change is created.

## Core Principles

- Prefer readable code over clever code.
- Keep behavior-preserving refactors separate from behavior changes.
- Keep functions small and focused.
- Keep classes cohesive and named around a clear responsibility.
- Make ownership and lifetime obvious.
- Make parser and runtime errors explicit and easy to diagnose.
- Preserve existing behavior unless the change intentionally and clearly documents otherwise.

## Formatting

CursedMenu uses `.clang-format` to define the project formatting style.

Use:

```bash
clang-format -i path/to/file.cpp path/to/file.hpp
```

Guidelines:

- Use 4 spaces for indentation.
- Do not use tabs for indentation.
- Use attached braces:

```cpp
if (condition) {
    doWork();
}
```

- Keep line lengths reasonable. The project target is 100 columns.
- Put spaces around operators.
- Prefer one declaration per line when it improves readability.
- Do not mix broad formatting-only changes with behavior changes.
- If a large existing file needs full reformatting, do it in a dedicated formatting-only commit.

## Naming

Use names that describe intent.

Recommended naming:

- Classes and structs: `PascalCase`
  - `CursedMenuRunner`
  - `MenuParserFactory`
  - `MenuDefinition`
- Functions and methods: `camelCase`
  - `parseFile`
  - `supportsFile`
  - `runMenu`
- Variables: `camelCase`
  - `menuFile`
  - `parseResult`
- Constants: clear descriptive names; prefer scoped constants over macros.
- File names should match the main class when practical.

Avoid:

- Single-letter names except for very small loop counters.
- Abbreviations that are not obvious.
- Names that describe implementation details rather than purpose.

## Header and Source Files

- Prefer `.hpp` for C++ headers in new code unless an existing directory convention clearly uses something else.
- Prefer `.cpp` for C++ implementation files in new code.
- Keep public interfaces small and documented.
- Avoid `using namespace std;` in headers.
- Avoid broad `using namespace std;` in implementation files when reasonable.
- Include only what the file needs.
- Prefer forward declarations in headers when possible and readable.

## Comments and Documentation

Comments should explain why code exists, not merely repeat what it does.

Guidelines:

- Keep existing license and copyright headers intact.
- Add file header comments where appropriate.
- Document public classes and public methods.
- Document non-obvious parser behavior.
- Document terminal/ncurses lifecycle assumptions.
- Document exception-safety boundaries.
- Update comments when behavior changes.
- Do not leave commented-out debug code in committed changes.

Example:

```cpp
/**
 * Parses a menu definition file into the common runtime menu model.
 *
 * Parser implementations should validate file-format-specific rules and return
 * structured errors instead of terminating the application.
 */
class MenuParser {
public:
    virtual ~MenuParser() = default;
};
```

## C++ Best Practices

Use modern C++ practices that improve safety and clarity.

Guidelines:

- Prefer RAII over manual setup/teardown.
- Prefer stack allocation when ownership is local.
- Use `std::unique_ptr` for exclusive dynamic ownership.
- Avoid raw owning pointers.
- Prefer references for required dependencies.
- Prefer `std::filesystem::path` for file paths.
- Prefer `std::string` and standard containers over manual buffers.
- Prefer `nullptr` over `NULL` or `0` for pointers.
- Use `const` where it improves correctness and readability.
- Avoid macros for constants when `constexpr`, `const`, or enum classes are better.
- Avoid unchecked indexing when parsing untrusted input.

## Memory Safety

Modernized code should avoid memory leaks and unsafe buffer behavior.

Requirements:

- Do not introduce raw owning pointers.
- Do not manually allocate memory unless there is a clear reason.
- Avoid C-style string copying and fixed-size buffers.
- Use standard containers and strings for storage.
- Ensure ncurses lifecycle handling is exception-safe.
- Use sanitizers or Valgrind to check ownership-sensitive changes when practical.

## Input and Parser Safety

Menu files should be treated as untrusted input.

Requirements:

- Malformed JSON or `.cmd` files must not crash the application.
- Parser failures should return structured errors.
- Parser errors should include file name and line/column or JSON path when practical.
- Recursive submenu loading must detect cycles.
- Deep recursion should be limited or guarded.
- Unknown colors, missing item names, invalid commands, and missing menu terminators should be reported clearly.

## Exception Handling

Use a consistent error-handling model.

Guidelines:

- Use structured result types for expected validation failures.
- Use exceptions for exceptional runtime failures.
- Catch exceptions at the application boundary.
- Ensure terminal state is restored before the process exits.
- Return meaningful process exit codes.
- Log enough context to troubleshoot failures.
- Do not use `exit()` deep inside helper classes or parser code.

Expected boundary pattern:

```cpp
int main(int argc, char** argv) {
    try {
        CursedMenuApp app;
        return app.run(argc, argv);
    } catch (const std::exception& ex) {
        // Log/report failure and return a non-zero exit code.
        return 1;
    } catch (...) {
        return 1;
    }
}
```

## Parser Architecture Standards

Menu parsing should support JSON, legacy `.cmd`, and future formats without leaking file-format details into runtime code.

Guidelines:

- Use a common `MenuParser` contract.
- Implement `JsonMenuParser` and `LegacyCmdMenuParser` separately.
- Use `MenuParserFactory` or a registry to select the parser.
- Return a common `MenuDefinition` model.
- Keep parser code independent of ncurses.
- Keep shared semantic validation in `MenuValidator`.
- Use templates only when they make the code clearer or safer.
- Prefer simple runtime polymorphism when it is easier to read and maintain.

## ncurses and Runtime Boundaries

The executable entry point should not own ncurses behavior.

Guidelines:

- Keep ncurses references out of `main.cpp`.
- Move terminal runtime behavior into `libCursedMenu`.
- Use `CursedMenuRunner` for menu rendering and input handling.
- Use an RAII wrapper such as `TerminalSession` for ncurses setup and teardown.
- Keep parser and validation logic testable without ncurses.

## Tests

Add tests where practical for new behavior and risky refactors.

Priority areas:

- Command-line parsing.
- JSON parser success and failure cases.
- Legacy `.cmd` parser success and failure cases.
- Parser factory selection.
- Malformed input handling.
- Submenu recursion and cycle detection.
- Exception boundary behavior.

## Review Checklist

Before merging modernization work, confirm:

- The app still builds.
- Existing behavior is preserved or intentional behavior changes are documented.
- The change is small and focused.
- New or touched code follows formatting standards.
- Public classes and methods are documented.
- Ownership is clear and safe.
- Parser/input handling rejects malformed input without crashing.
- Exceptions are handled at appropriate boundaries.
- User-facing changes are documented.
