# CursedMenu Modernization Plan

## Purpose

This document captures the working plan for modernizing CursedMenu while preserving the original goal: a lightweight ncurses-based menu system for character terminal sessions such as SSH, telnet, and other text-based clients.

The modernization effort should favor small, reviewable changes that keep the app usable while improving maintainability, safety, testability, and packaging.

## Working Branch

All modernization work for this effort should be based on:

```text
modernization-2026-05-05
```

The branch was created from `dev`.

## Current Observations

The initial code review surfaced these areas:

1. The application has a large terminal UI entry point in `source/cursedmenu/main.cpp`.
2. The project has a CMake build file at the repository root.
3. The app has existing command-line behavior for selecting a menu file and checking/parsing menu definitions.
4. Historical code includes a recursive menu loader for submenu discovery.
5. Recent code added TERMINFO environment handling directly near application startup.
6. Some older code paths appear to use raw pointers, global-style helper functions, manual argument parsing, direct process exits, and mixed UI/business logic.
7. Logging and diagnostic output appear to be early-stage and should be made more consistent before deeper refactoring.
8. `main.cpp` should not directly own ncurses setup, teardown, or runtime menu handling. Those responsibilities should move into `libCursedMenu`.
9. Existing code should be reviewed for consistent comments, naming, formatting, and maintainability as modernization proceeds.
10. The existing custom `.cmd` menu definition format should be migrated toward a standard structured format, with JSON as the preferred and default target.

## Modernization Goals

### 1. Preserve Existing Behavior

Do not start with a rewrite. The first goal is to preserve the current user-facing behavior while creating seams for safer improvement.

Guidelines:

- Keep the default menu flow working.
- Make JSON the default menu definition format for new work.
- Keep existing `.cmd` menu definition files compatible during the migration period.
- Keep command-line options compatible unless a change is intentionally documented.
- Prefer additive changes over breaking changes.
- Provide conversion tooling before removing support for older `.cmd` files.
- Treat `.cmd` as deprecated once JSON loading is stable and documented.

### 2. Improve Build and Developer Workflow

Goals:

- Confirm the app builds cleanly from a fresh checkout.
- Standardize build instructions in `README.md`.
- Add a predictable out-of-source build workflow.
- Add compiler warnings.
- Add a basic CI workflow once the local build is stable.

Suggested tasks:

- Add or confirm CMake minimum version and project metadata.
- Add warning flags for GCC/Clang.
- Add Debug and Release build examples.
- Document required dependencies, especially ncurses.
- Add a simple GitHub Actions build for Ubuntu.

### 3. Separate Application Responsibilities

The main application entry point should eventually become thin and delegate to focused components.

Suggested target structure:

- `main.cpp` for minimal executable startup only.
- `App` or `CursedMenuApp` for top-level orchestration.
- `CommandLineOptions` for argument parsing.
- `Environment` or `TerminalEnvironment` for TERMINFO and terminal setup.
- `MenuRepository` or `MenuLoader` for reading menu definitions.
- `MenuDefinition` or similar domain model for parsed menu data independent of file format.
- `JsonMenuLoader` for JSON menu files.
- `LegacyCmdMenuLoader` for existing `.cmd` menu files during migration.
- `CursedMenuRunner` in `libCursedMenu` for ncurses setup, menu rendering, input handling, and cleanup.
- `Logger` for application logging.

Guiding direction:

- `main.cpp` should not include or directly depend on ncurses headers.
- ncurses-specific calls should be isolated behind `CursedMenuRunner` or closely related terminal UI classes.
- The executable should call into `libCursedMenu` instead of owning terminal behavior itself.
- Non-terminal logic such as parsing, validation, and option handling should remain testable without initializing ncurses.
- Menu loading should produce a common in-memory model regardless of whether the source file is JSON or legacy `.cmd`.

Suggested tasks:

- Extract command-line parsing from `main.cpp`.
- Extract TERMINFO handling from `main.cpp`.
- Create `CursedMenuRunner` in `libCursedMenu`.
- Move ncurses setup and teardown into `CursedMenuRunner`.
- Move menu event loop behavior into `CursedMenuRunner`.
- Create a menu definition domain model that is independent of input file format.
- Replace direct `exit()` calls in helper functions with return codes or exceptions.
- Create smaller functions/classes before changing behavior.

### 4. Make Memory Ownership Safer

Where code uses raw owning pointers, modernize toward stack allocation or smart pointers.

Suggested tasks:

- Replace obvious `new` allocations with stack objects where lifetime is local.
- Use `std::unique_ptr` only where dynamic allocation is required.
- Avoid passing raw owning pointers between components.
- Prefer references for required dependencies and pointers only for optional values.

### 5. Standardize Menu Files on JSON

The long-term target menu format should be JSON. JSON is widely understood, easy to validate, easy to generate, and easier to document than a custom parser format.

Policy:

- JSON is the default menu file format for new menus.
- The application should prefer JSON examples, JSON documentation, and JSON validation behavior.
- The legacy `.cmd` format remains supported for the time being.
- The legacy `.cmd` format is deprecated and should eventually be removed after users have a documented migration path.
- Deprecation should be visible but not disruptive at first, such as a warning when loading `.cmd` files.

Goals:

- Define a versioned JSON schema for menu files.
- Support JSON menu files as the preferred default format.
- Keep legacy `.cmd` support temporarily so existing users are not broken.
- Provide a conversion path from `.cmd` to JSON.
- Make parser behavior testable without ncurses.

Suggested JSON shape:

```json
{
  "version": 1,
  "settings": {
    "debug": false,
    "pauseAfterExecution": false
  },
  "menus": [
    {
      "id": "main",
      "title": "Main Menu",
      "foreground": "WHITE",
      "background": "BLUE",
      "items": [
        {
          "name": "System Info",
          "description": "Show system information",
          "command": "uname -a"
        },
        {
          "name": "Utilities",
          "description": "Open the utilities submenu",
          "submenu": "utilities"
        },
        {
          "name": "Exit",
          "description": "Exit this menu",
          "action": "exit"
        }
      ]
    },
    {
      "id": "utilities",
      "title": "Utilities",
      "foreground": "WHITE",
      "background": "BLACK",
      "items": []
    }
  ],
  "rootMenu": "main"
}
```

Design guidelines:

- Use a top-level `version` field so future changes can be handled safely.
- Use stable menu IDs instead of relying only on file names.
- Represent commands and submenus explicitly instead of overloading command strings such as `MenuSub ...`.
- Represent exit behavior as a structured action.
- Keep colors as readable strings, but validate them against known supported values.
- Support comments through documentation rather than non-standard JSON comments.

Validation rules:

- `version` is required.
- `rootMenu` is required and must match a menu ID.
- Menu IDs must be unique.
- Item names should not be empty.
- Each item should have exactly one behavior: `command`, `submenu`, or `action`.
- Submenu references must point to existing menu IDs.
- Submenu cycles should be detected and reported clearly.
- Unknown colors should produce validation errors.
- Validation errors should include file name and JSON path when practical.

Migration and deprecation plan:

1. Document the JSON format in `docs/menu-format.md`.
2. Add JSON examples alongside existing `.cmd` examples.
3. Introduce `JsonMenuLoader` that maps JSON into the common menu model.
4. Make default generated/sample menu files JSON.
5. Update the default menu file lookup to prefer JSON, such as `default.json`, while falling back to legacy `.cmd` only during the migration window.
6. Keep `LegacyCmdMenuLoader` or equivalent for the existing format.
7. Add a converter utility or command-line option to convert `.cmd` files to JSON.
8. Update validation/check mode to support both JSON and `.cmd` files.
9. Emit a deprecation warning when loading `.cmd` files after JSON support is stable.
10. Prefer JSON in documentation and examples.
11. Remove `.cmd` support only in a future major/versioned change after deprecation has been documented.

Dependency note:

- Evaluate whether to use a well-known single-header JSON library, such as `nlohmann/json`, or a smaller C++ JSON parser.
- Avoid adding a dependency until the build and packaging impact is reviewed.
- If adding a dependency, document it clearly in build instructions and CI.

### 6. Improve Legacy Menu File Parsing

The existing `.cmd` format remains important during migration and should be maintained until JSON is stable.

Suggested tasks:

- Document the current `.cmd` menu file format as legacy and deprecated.
- Create parser tests using existing sample `.cmd` files.
- Add validation errors with file name and line number.
- Detect missing `MenuEnd`, missing `ItemEnd`, unknown colors, empty item names, and missing commands.
- Guard recursive submenu loading against cycles.
- Keep support for existing `MenuSub` behavior during the deprecation window.
- Add conversion tests to verify `.cmd` files convert into equivalent JSON menu definitions.
- Add deprecation notices in documentation and runtime warnings when appropriate.

### 7. Improve Logging and Diagnostics

Logging should help diagnose terminal and menu definition issues without noisy output in normal use.

Suggested tasks:

- Centralize log level handling.
- Route debug messages through the logger rather than direct `cout`/`cerr` calls.
- Make log file location configurable.
- Add clear startup diagnostics when terminal setup fails.
- Avoid unused return values from environment or setup calls.

### 8. Improve Terminal Environment Handling

TERMINFO handling is important, but should be isolated and testable.

Suggested tasks:

- Move TERMINFO helpers into a terminal environment module.
- Keep TERMINFO/environment concerns separate from ncurses runtime concerns.
- Check return codes from environment setup functions.
- Avoid silently ignoring setup failures.
- Document why `/usr/share/terminfo` is used as the fallback.
- Consider allowing an override through command-line options or config.

### 9. Move ncurses Runtime into libCursedMenu

The ncurses dependency should be owned by the reusable CursedMenu library layer, not the executable entry point.

Proposed class:

```text
CursedMenuRunner
```

Proposed responsibilities:

- Initialize ncurses.
- Configure colors and terminal display options.
- Own the menu rendering loop.
- Handle keyboard input and navigation.
- Dispatch menu commands.
- Display item descriptions and status text.
- Clean up ncurses safely on normal exit and error exit.

Non-responsibilities:

- Command-line parsing.
- Reading process environment variables directly, except through injected terminal/environment helpers.
- Parsing menu definition files.
- Deciding which menu file should be loaded.

Suggested public shape:

```cpp
class CursedMenuRunner {
public:
    explicit CursedMenuRunner(Logger& logger);

    int run(const MenuDefinition& menuDefinition);
};
```

The final design may change after inspecting existing class names and build layout, but the direction should remain: `main.cpp` delegates runtime behavior and ncurses references to `libCursedMenu`.

### 10. Establish C++ Formatting and Comment Standards

Modernization should improve both new code and existing code readability. Every change should follow a documented C++ style, and existing touched files should be cleaned up opportunistically without creating noisy, unrelated rewrites.

Formatting guidelines:

- Use a consistent C++ style across headers and implementation files.
- Prefer 4-space indentation; do not use tabs for indentation.
- Keep braces consistent. Prefer opening braces on the same line for functions, conditionals, and loops unless the project adopts a different explicit standard.
- Keep line lengths reasonable, targeting readability over rigid wrapping.
- Prefer `nullptr` over `NULL` or `0` for pointers.
- Prefer `const` correctness where practical.
- Prefer standard C++ library types and RAII over manual resource handling.
- Avoid broad `using namespace std;` in headers. Remove it from implementation files when reasonable.
- Use clear names that describe intent.

Comment and documentation guidelines:

- Add file header comments where appropriate and keep existing license/copyright headers intact.
- Public classes and public methods should have clear documentation comments.
- Prefer comments that explain why code exists, not comments that merely repeat what the code does.
- Complex parsing, ncurses lifecycle handling, and terminal/environment assumptions should be documented.
- Keep comments current when behavior changes.
- Avoid leaving commented-out debug code in committed changes.

Review guidelines for existing code:

- When touching an existing file, review nearby code for formatting inconsistencies, stale comments, unsafe ownership, and unclear naming.
- Do not perform massive formatting-only rewrites mixed with behavior changes.
- If broad formatting cleanup is needed, do it in a separate commit.
- Add TODO comments only when they are specific, actionable, and preferably tied to a plan item.

Suggested tasks:

- Add a `docs/coding-standards.md` document.
- Decide whether to add `.clang-format`.
- If `.clang-format` is added, apply it gradually or in a dedicated formatting-only commit.
- Review existing headers and source files for stale comments and inconsistent style.
- Update public API comments as classes are extracted.

### 11. Add Tests Incrementally

Start with tests around logic that does not require an interactive terminal.

Suggested initial tests:

- Command-line option parsing.
- Environment variable helper behavior.
- JSON menu parser success cases.
- JSON menu parser validation failure cases.
- Legacy `.cmd` parser success cases.
- Legacy `.cmd` parser failure cases.
- Legacy `.cmd` to JSON conversion.
- Recursive submenu loading.
- Cycle detection in submenu loading.

Later tests:

- Non-interactive smoke test for app startup.
- Snapshot-style output test for `--help`.
- Integration tests using sample menu definitions.

### 12. Improve Documentation

Suggested documentation updates:

- Build from source.
- Install dependencies.
- Run the app.
- Run the menu validation/check mode.
- JSON menu definition syntax.
- Legacy `.cmd` menu definition syntax.
- Example menu files.
- Migration guide from `.cmd` to JSON.
- Deprecation timeline and compatibility notes for `.cmd` support.
- Troubleshooting terminal/TERMINFO problems.
- Project roadmap.
- Library/executable boundary, including the role of `CursedMenuRunner`.
- C++ coding standards and commenting expectations.

### 13. Prepare for Packaging

Longer-term packaging goals:

- Install target through CMake.
- Install example menu files.
- Generate or include man page documentation.
- Version the executable consistently.
- Consider packaging for Debian/Ubuntu if useful.

## Suggested Improvement Backlog

### Phase 1: Stabilize and Document

- Add `APP_PLAN.md`.
- Update `README.md` with build/run/check instructions.
- Add a `docs/menu-format.md` document covering the default JSON format and deprecated legacy `.cmd` format.
- Add a `docs/developer-workflow.md` document.
- Add a `docs/coding-standards.md` document.
- Confirm the project builds on current Ubuntu with ncurses installed.

### Phase 2: Build and CI Hygiene

- Modernize `CMakeLists.txt` without changing app behavior.
- Add compiler warning flags.
- Add GitHub Actions CI for build verification.
- Add a basic formatting/linting decision, even if enforcement waits.
- Decide whether to introduce `.clang-format`.
- Decide on the JSON library/dependency strategy.

### Phase 3: Menu Model and JSON Support

- Create a common menu domain model independent of file format.
- Add JSON menu examples.
- Add `JsonMenuLoader`.
- Make JSON the default menu format.
- Add JSON validation tests.
- Keep legacy `.cmd` loading functional during the deprecation window.
- Add a `.cmd` to JSON conversion path.
- Add deprecation warnings for `.cmd` usage after JSON loading is stable.

### Phase 4: Extract Low-Risk Helpers

- Extract command-line parsing.
- Extract terminal environment setup.
- Create `CursedMenuRunner` in `libCursedMenu`.
- Move ncurses setup/teardown and menu runtime behavior out of `main.cpp`.
- Extract app startup orchestration.
- Remove unused variables and ignored return values where safe.
- Review touched code for comment quality and C++ formatting consistency.

### Phase 5: Parser Improvements

- Add parser tests.
- Add parser error reporting with line numbers or JSON paths.
- Add submenu recursion guard.
- Improve handling of invalid menu files.
- Document parser behavior and edge cases.

### Phase 6: Runtime/UI Improvements

- Improve menu navigation behavior.
- Improve status/help text.
- Improve terminal resize handling if needed.
- Improve error presentation for failed commands.

## Coding Guidelines

- Prefer small commits with one purpose.
- Preserve behavior unless the commit message says otherwise.
- Favor clear names over clever code.
- Prefer standard C++ library features over custom helpers where practical.
- Avoid introducing new dependencies unless they solve a clear problem.
- Keep terminal-specific code isolated from parsing and app logic.
- Keep ncurses references out of `main.cpp`; terminal runtime should live in `libCursedMenu`.
- Treat JSON as the default and preferred menu format for new work.
- Keep legacy `.cmd` support working during the deprecation window.
- Do not add new `.cmd` examples unless specifically documenting migration or legacy compatibility.
- Follow the project C++ formatting and comment standards for all new changes.
- Review existing touched code for formatting, comments, ownership, and maintainability.
- Add tests before or during behavior changes.
- Update documentation with user-visible changes.

## Definition of Done for Modernization Tasks

A task is done when:

- The app still builds.
- Existing behavior is preserved or intentional behavior changes are documented.
- New logic has tests when practical.
- JSON menu format changes include documentation and examples.
- Any `.cmd` behavior changes include compatibility and deprecation notes.
- New or touched code follows the project C++ formatting and comment standards.
- Public classes and public methods have useful documentation comments.
- User-facing changes are reflected in documentation.
- The change is small enough to review confidently.

## Near-Term Recommended Next Tasks

1. Update `README.md` with current build and run instructions.
2. Add menu format documentation with default JSON syntax and deprecated legacy `.cmd` notes.
3. Add coding standards documentation and decide on `.clang-format`.
4. Decide on a JSON parser dependency strategy.
5. Create a common menu domain model.
6. Add JSON menu examples.
7. Make the app prefer a JSON default menu file while keeping `.cmd` fallback support.
8. Extract command-line parsing from `main.cpp`.
9. Extract TERMINFO environment handling from `main.cpp`.
10. Create `CursedMenuRunner` in `libCursedMenu` and move ncurses runtime behavior out of `main.cpp`.
11. Add a first test target for non-interactive logic.
12. Add CI that builds the project on Ubuntu.

## Notes

This plan should evolve as the codebase is reviewed more deeply. Each completed modernization task should either update this plan or remove items that are no longer relevant.
