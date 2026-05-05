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

## Modernization Goals

### 1. Preserve Existing Behavior

Do not start with a rewrite. The first goal is to preserve the current user-facing behavior while creating seams for safer improvement.

Guidelines:

- Keep the default menu flow working.
- Keep existing menu definition files compatible.
- Keep command-line options compatible unless a change is intentionally documented.
- Prefer additive changes over breaking changes.

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
- `CursedMenuRunner` in `libCursedMenu` for ncurses setup, menu rendering, input handling, and cleanup.
- `Logger` for application logging.

Guiding direction:

- `main.cpp` should not include or directly depend on ncurses headers.
- ncurses-specific calls should be isolated behind `CursedMenuRunner` or closely related terminal UI classes.
- The executable should call into `libCursedMenu` instead of owning terminal behavior itself.
- Non-terminal logic such as parsing, validation, and option handling should remain testable without initializing ncurses.

Suggested tasks:

- Extract command-line parsing from `main.cpp`.
- Extract TERMINFO handling from `main.cpp`.
- Create `CursedMenuRunner` in `libCursedMenu`.
- Move ncurses setup and teardown into `CursedMenuRunner`.
- Move menu event loop behavior into `CursedMenuRunner`.
- Replace direct `exit()` calls in helper functions with return codes or exceptions.
- Create smaller functions/classes before changing behavior.

### 4. Make Memory Ownership Safer

Where code uses raw owning pointers, modernize toward stack allocation or smart pointers.

Suggested tasks:

- Replace obvious `new` allocations with stack objects where lifetime is local.
- Use `std::unique_ptr` only where dynamic allocation is required.
- Avoid passing raw owning pointers between components.
- Prefer references for required dependencies and pointers only for optional values.

### 5. Improve Menu File Parsing

Menu definition parsing is core to the application and should become one of the best-tested areas.

Suggested tasks:

- Document the menu file format.
- Create parser tests using existing sample `.cmd` files.
- Add validation errors with file name and line number.
- Detect missing `MenuEnd`, missing `ItemEnd`, unknown colors, empty item names, and missing commands.
- Guard recursive submenu loading against cycles.
- Keep support for existing `MenuSub` behavior.

### 6. Improve Logging and Diagnostics

Logging should help diagnose terminal and menu definition issues without noisy output in normal use.

Suggested tasks:

- Centralize log level handling.
- Route debug messages through the logger rather than direct `cout`/`cerr` calls.
- Make log file location configurable.
- Add clear startup diagnostics when terminal setup fails.
- Avoid unused return values from environment or setup calls.

### 7. Improve Terminal Environment Handling

TERMINFO handling is important, but should be isolated and testable.

Suggested tasks:

- Move TERMINFO helpers into a terminal environment module.
- Keep TERMINFO/environment concerns separate from ncurses runtime concerns.
- Check return codes from environment setup functions.
- Avoid silently ignoring setup failures.
- Document why `/usr/share/terminfo` is used as the fallback.
- Consider allowing an override through command-line options or config.

### 8. Move ncurses Runtime into libCursedMenu

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

    int run(const MenuConfig& rootMenu);
};
```

The final design may change after inspecting existing class names and build layout, but the direction should remain: `main.cpp` delegates runtime behavior and ncurses references to `libCursedMenu`.

### 9. Establish C++ Formatting and Comment Standards

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

### 10. Add Tests Incrementally

Start with tests around logic that does not require an interactive terminal.

Suggested initial tests:

- Command-line option parsing.
- Environment variable helper behavior.
- Menu file parser success cases.
- Menu file parser failure cases.
- Recursive submenu loading.
- Cycle detection in submenu loading.

Later tests:

- Non-interactive smoke test for app startup.
- Snapshot-style output test for `--help`.
- Integration tests using sample menu definitions.

### 11. Improve Documentation

Suggested documentation updates:

- Build from source.
- Install dependencies.
- Run the app.
- Run the menu validation/check mode.
- Menu definition syntax.
- Example menu files.
- Troubleshooting terminal/TERMINFO problems.
- Project roadmap.
- Library/executable boundary, including the role of `CursedMenuRunner`.
- C++ coding standards and commenting expectations.

### 12. Prepare for Packaging

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
- Add a `docs/menu-format.md` document.
- Add a `docs/developer-workflow.md` document.
- Add a `docs/coding-standards.md` document.
- Confirm the project builds on current Ubuntu with ncurses installed.

### Phase 2: Build and CI Hygiene

- Modernize `CMakeLists.txt` without changing app behavior.
- Add compiler warning flags.
- Add GitHub Actions CI for build verification.
- Add a basic formatting/linting decision, even if enforcement waits.
- Decide whether to introduce `.clang-format`.

### Phase 3: Extract Low-Risk Helpers

- Extract command-line parsing.
- Extract terminal environment setup.
- Create `CursedMenuRunner` in `libCursedMenu`.
- Move ncurses setup/teardown and menu runtime behavior out of `main.cpp`.
- Extract app startup orchestration.
- Remove unused variables and ignored return values where safe.
- Review touched code for comment quality and C++ formatting consistency.

### Phase 4: Parser Improvements

- Add parser tests.
- Add parser error reporting with line numbers.
- Add submenu recursion guard.
- Improve handling of invalid menu files.
- Document parser behavior and edge cases.

### Phase 5: Runtime/UI Improvements

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
- Follow the project C++ formatting and comment standards for all new changes.
- Review existing touched code for formatting, comments, ownership, and maintainability.
- Add tests before or during behavior changes.
- Update documentation with user-visible changes.

## Definition of Done for Modernization Tasks

A task is done when:

- The app still builds.
- Existing behavior is preserved or intentional behavior changes are documented.
- New logic has tests when practical.
- New or touched code follows the project C++ formatting and comment standards.
- Public classes and public methods have useful documentation comments.
- User-facing changes are reflected in documentation.
- The change is small enough to review confidently.

## Near-Term Recommended Next Tasks

1. Update `README.md` with current build and run instructions.
2. Add menu format documentation based on the sample `.cmd` files.
3. Add coding standards documentation and decide on `.clang-format`.
4. Extract command-line parsing from `main.cpp`.
5. Extract TERMINFO environment handling from `main.cpp`.
6. Create `CursedMenuRunner` in `libCursedMenu` and move ncurses runtime behavior out of `main.cpp`.
7. Add a first test target for non-interactive logic.
8. Add CI that builds the project on Ubuntu.

## Notes

This plan should evolve as the codebase is reviewed more deeply. Each completed modernization task should either update this plan or remove items that are no longer relevant.
