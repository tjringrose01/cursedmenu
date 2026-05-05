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

- `App` or `CursedMenuApp` for top-level orchestration.
- `CommandLineOptions` for argument parsing.
- `Environment` or `TerminalEnvironment` for TERMINFO and terminal setup.
- `MenuRepository` or `MenuLoader` for reading menu definitions.
- `MenuRunner` for ncurses interaction and event loop.
- `Logger` for application logging.

Suggested tasks:

- Extract command-line parsing from `main.cpp`.
- Extract TERMINFO handling from `main.cpp`.
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
- Check return codes from environment setup functions.
- Avoid silently ignoring setup failures.
- Document why `/usr/share/terminfo` is used as the fallback.
- Consider allowing an override through command-line options or config.

### 8. Add Tests Incrementally

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

### 9. Improve Documentation

Suggested documentation updates:

- Build from source.
- Install dependencies.
- Run the app.
- Run the menu validation/check mode.
- Menu definition syntax.
- Example menu files.
- Troubleshooting terminal/TERMINFO problems.
- Project roadmap.

### 10. Prepare for Packaging

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
- Confirm the project builds on current Ubuntu with ncurses installed.

### Phase 2: Build and CI Hygiene

- Modernize `CMakeLists.txt` without changing app behavior.
- Add compiler warning flags.
- Add GitHub Actions CI for build verification.
- Add a basic formatting/linting decision, even if enforcement waits.

### Phase 3: Extract Low-Risk Helpers

- Extract command-line parsing.
- Extract terminal environment setup.
- Extract app startup orchestration.
- Remove unused variables and ignored return values where safe.

### Phase 4: Parser Improvements

- Add parser tests.
- Add parser error reporting with line numbers.
- Add submenu recursion guard.
- Improve handling of invalid menu files.

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
- Add tests before or during behavior changes.
- Update documentation with user-visible changes.

## Definition of Done for Modernization Tasks

A task is done when:

- The app still builds.
- Existing behavior is preserved or intentional behavior changes are documented.
- New logic has tests when practical.
- User-facing changes are reflected in documentation.
- The change is small enough to review confidently.

## Near-Term Recommended Next Tasks

1. Update `README.md` with current build and run instructions.
2. Add menu format documentation based on the sample `.cmd` files.
3. Extract command-line parsing from `main.cpp`.
4. Extract TERMINFO environment handling from `main.cpp`.
5. Add a first test target for non-interactive logic.
6. Add CI that builds the project on Ubuntu.

## Notes

This plan should evolve as the codebase is reviewed more deeply. Each completed modernization task should either update this plan or remove items that are no longer relevant.
