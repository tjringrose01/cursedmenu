# CursedMenu

CursedMenu is an ncurses-based menu system for character terminal sessions. It is intended for text-based clients such as SSH, telnet, rlogin, or local terminal sessions where a lightweight interactive menu can be useful for user tools, system administration tasks, or utility launchers.

## Modernization Status

This repository is being modernized on the following working branch:

```text
modernization-2026-05-05
```

The modernization roadmap is tracked in [`APP_PLAN.md`](APP_PLAN.md). That plan captures the current goals around maintainability, JSON menu files, parser architecture, ncurses runtime separation, memory safety, testing, and CI.

## Current Goals

The current modernization effort focuses on:

- Keeping existing behavior working while improving the codebase.
- Making JSON the default menu definition format for new work.
- Continuing to support legacy `.cmd` menu files during a deprecation period.
- Moving ncurses runtime behavior out of `main.cpp` and into `libCursedMenu`.
- Improving readability, maintainability, comments, and C++ formatting.
- Adding safer memory ownership, exception handling, and parser validation.
- Adding automated tests and CI.

## Dependencies

A typical Linux development environment needs:

- A C++ compiler such as `g++` or `clang++`.
- CMake.
- ncurses development headers and libraries.
- make or another CMake-supported build tool.

On Ubuntu or Debian-based systems, install the common dependencies with:

```bash
sudo apt update
sudo apt install build-essential cmake libncurses-dev
```

## Build from Source

Use an out-of-source build so generated files stay out of the source tree.

```bash
git clone https://github.com/tjringrose01/cursedmenu.git
cd cursedmenu
git checkout modernization-2026-05-05
cmake -S . -B build
cmake --build build
```

## Debug Build

```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build-debug
```

## Release Build

```bash
cmake -S . -B build-release -DCMAKE_BUILD_TYPE=Release
cmake --build build-release
```

## Run the Application

The exact executable path may depend on the current CMake target layout. After building, look under the build directory for the generated executable.

Example:

```bash
./build/cursedmenu
```

If your build places the executable under a nested source directory, run it from there instead.

## Menu Files

### Preferred Format: JSON

JSON is the planned default menu definition format for new menus. JSON support is part of the modernization roadmap and should become the preferred format for examples, documentation, validation, and future work.

The planned shape is similar to:

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
          "name": "Exit",
          "description": "Exit this menu",
          "action": "exit"
        }
      ]
    }
  ],
  "rootMenu": "main"
}
```

### Legacy Format: `.cmd`

The existing `.cmd` menu format remains supported for now, but it is considered deprecated as part of the modernization plan. Existing `.cmd` files should continue to work during the migration period. Future work should include conversion tooling and deprecation warnings.

## Run with a Menu File

Use the `-m` option to specify a menu definition file.

```bash
./build/cursedmenu -m default.cmd
```

As JSON support is implemented, new examples should prefer JSON:

```bash
./build/cursedmenu -m default.json
```

## Validate or Check Menu Definitions

The application has existing support for a check mode using `-c`. This mode is intended to parse menu definitions and display validation/check output without running the interactive menu.

```bash
./build/cursedmenu -m default.cmd -c
```

As JSON support is added, validation should support both JSON and legacy `.cmd` files:

```bash
./build/cursedmenu -m default.json -c
```

## Help

Use `-h` or `--help` to display command-line usage when supported by the current executable.

```bash
./build/cursedmenu --help
```

## Troubleshooting

### ncurses or terminal errors

If the app fails to start because of terminal database issues, verify that ncurses is installed and that the TERM and TERMINFO environment variables are reasonable for your system.

Common checks:

```bash
echo "$TERM"
echo "$TERMINFO"
ls /usr/share/terminfo
```

The modernization plan includes moving terminal environment handling into a dedicated component so startup errors become easier to diagnose.

### Executable path differs from examples

The current CMake target layout may place the executable somewhere other than `./build/cursedmenu`. Use `find` if needed:

```bash
find build -type f -executable
```

## Developer Workflow

Recommended workflow for modernization changes:

1. Create or use a feature branch based on `dev`.
2. Keep changes small and reviewable.
3. Preserve existing behavior unless the change intentionally modifies behavior.
4. Update documentation for user-visible changes.
5. Add tests for parser, validation, and non-interactive logic when practical.
6. Follow the project coding standards once documented.
7. Avoid introducing new dependencies without documenting build and packaging impact.

## Modernization Backlog

Modernization work is tracked in GitHub issues. Current workstreams include:

- README and developer workflow documentation.
- Coding standards and formatting.
- JSON menu format and parser architecture.
- Moving ncurses runtime into `libCursedMenu`.
- Memory safety, sanitizers, and exception handling.
- CI, compiler warnings, and developer tooling.
- Parser validation and `.cmd` to JSON migration support.
- Automated tests and non-interactive coverage.
- Runtime UX, diagnostics, and terminal handling.

## License

CursedMenu has historically been distributed under the GNU General Public License. Preserve existing license and copyright headers when modernizing source files.
