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

Install Conan:

```bash
pip install conan
```

Detect a local Conan profile:

```bash
conan profile detect --force
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
