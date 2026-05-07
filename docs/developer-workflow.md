# CursedMenu Developer Workflow

## Purpose

This document describes the recommended development workflow for the ongoing CursedMenu modernization effort.

The goals are:

- Consistent local builds
- Reliable CI builds
- Small reviewable commits
- Safer modernization work
- Improved maintainability and readability
- Stable parser and runtime architecture evolution

## Branch Strategy

Modernization work should branch from:

```text
dev
```

Current active modernization branch:

```text
modernization-2026-05-05
```

Recommended workflow:

1. Create a focused feature branch.
2. Keep changes scoped to one concern.
3. Build locally before committing.
4. Let GitHub Actions verify the build.
5. Open a pull request into `dev`.

## Build System

The project uses:

- CMake
- Conan
- Ninja
- GitHub Actions CI

The repository now includes committed `CMakePresets.json` presets for consistent local and CI builds.

## Conan Setup

Install base build dependencies first.

Ubuntu / Debian:

```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential \
  cmake \
  ninja-build \
  python3 \
  python3-pip \
  libncurses-dev \
  clang-tidy \
  libasan8 \
  libubsan1
```

RHEL / Rocky / AlmaLinux / Fedora:

```bash
sudo dnf install -y \
  gcc \
  gcc-c++ \
  cmake \
  ninja-build \
  python3 \
  python3-pip \
  ncurses-devel \
  clang-tools-extra \
  libasan \
  libubsan
```

Install Conan:

```bash
python3 -m pip install --user conan
```

Detect a local Conan profile:

```bash
~/.local/bin/conan profile detect --force
```

## Recommended Local Build

The preferred local build workflow uses the helper script:

### Release Build

```bash
./tt
```

### Debug Build

```bash
./tt -d
```

The helper script:

- cleans previous build artifacts
- installs Conan dependencies
- configures CMake presets
- activates the Conan build environment
- builds the project

## Manual Build Workflow

### Release

```bash
conan install . --build=missing -s build_type=Release
cmake --preset=conan-release
cmake --build --preset=conan-release
```

### Debug

```bash
conan install . --build=missing -s build_type=Debug
cmake --preset=conan-debug
cmake --build --preset=conan-debug
```

## Current Parser Architecture Direction

The modernization effort is introducing a common parser architecture.

Current planned components:

- `MenuDefinition`
- `Menu`
- `MenuItem`
- `MenuAction`
- `MenuParser`
- `JsonMenuParser`
- `LegacyCmdMenuParser`
- `MenuParserFactory`
- `MenuParseResult`
- `MenuParseError`

Goals:

- JSON-first menu support
- Legacy `.cmd` compatibility during migration
- Separation of parser logic from ncurses runtime logic
- Structured parser validation
- Easier automated testing

## Runtime Architecture Direction

Long-term direction:

- `main.cpp` should remain minimal.
- ncurses runtime ownership should move into `libCursedMenu`.
- `CursedMenuRunner` should own terminal lifecycle and rendering.
- Parser logic should remain independent of ncurses.

Current ncurses runtime ownership boundaries:

- `NcursesSession`
  Handles `initscr()` / terminal mode setup and automatic teardown (`endwin()`),
  plus runtime color application and input reads.
- `NcursesWindow`
  Owns top-level `WINDOW*` lifecycle for the interactive frame.
- `NcursesMenu`
  Owns `MENU*`, `ITEM*`, and submenu `WINDOW*` allocations and cleanup.
- `MenuRenderer`
  Owns rendering responsibilities (frame setup, title/description drawing,
  and cursor/screen refresh behavior).
- `CursedMenuRunner`
  Orchestrates menu navigation and command/submenu flow while delegating
  ncurses ownership and rendering details to the abstractions above.

## Coding Standards

Follow:

- `docs/coding-standards.md`
- `.clang-format`

Format modified files using:

```bash
clang-format -i path/to/file.cpp
```

## Testing Expectations

Before submitting changes:

- Ensure the project builds locally.
- Ensure GitHub Actions CI passes.
- Add tests for parser or validation changes when practical.
- Verify malformed input does not crash the app.

Runtime manual verification checklist (ncurses flows):

- Launch the app and verify main menu renders correctly.
- Navigate up/down and confirm selection and description updates.
- Enter a submenu and return to parent menu.
- Run at least one command action and confirm terminal resumes correctly.
- Exit from submenu and root menu paths cleanly.

## CI Workflow

GitHub Actions currently verifies:

- Conan dependency installation
- CMake configuration
- Project compilation

Future CI goals:

- Automated tests
- Sanitizer builds
- Static analysis
- Formatting checks
- Coverage reporting

## Modernization Guidelines

- Prefer readable code over clever code.
- Preserve behavior unless intentionally changing behavior.
- Avoid introducing large unrelated refactors.
- Prefer RAII and modern C++ ownership patterns.
- Avoid raw owning pointers.
- Keep parser validation defensive.
- Keep commits small and reviewable.

## Documentation Expectations

Update documentation when:

- build workflow changes
- parser behavior changes
- menu format changes
- command-line behavior changes
- CI workflow changes
- user-visible runtime behavior changes

## Related Documents

- `README.md`
- `APP_PLAN.md`
- `docs/coding-standards.md`
