# CursedMenu

CursedMenu is an ncurses-based menu system for character terminal sessions. It is intended for text-based clients such as SSH, telnet, rlogin, or local terminal sessions where a lightweight interactive menu can be useful for user tools, system administration tasks, or utility launchers.

## Modernization Status

This repository is being modernized on the following working branch:

```text
modernization-2026-05-05
```

The modernization roadmap is tracked in [`APP_PLAN.md`](APP_PLAN.md). That plan captures the current goals around maintainability, JSON menu files, parser architecture, ncurses runtime separation, memory safety, testing, CI, and developer workflow modernization.

## Current Goals

The current modernization effort focuses on:

- Keeping existing behavior working while improving the codebase.
- Making JSON the default menu definition format for new work.
- Continuing to support legacy `.cmd` menu files during a deprecation period.
- Moving ncurses runtime behavior out of `main.cpp` and into `libCursedMenu`.
- Improving readability, maintainability, comments, and C++ formatting.
- Adding safer memory ownership, exception handling, and parser validation.
- Adding automated tests and CI.
- Standardizing Conan/CMake build workflows.

## Dependencies

A typical Linux development environment needs:

- A C++ compiler such as `g++` or `clang++`
- CMake
- Conan
- Ninja
- Python 3 and pip
- ncurses development headers and libraries
- clang-tidy (for static analysis / CI code-quality checks)
- AddressSanitizer and UndefinedBehaviorSanitizer runtime libraries
  (for sanitizer CI/builds)

On Ubuntu or Debian-based systems:

```bash
sudo apt update
sudo apt install -y \
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

On RHEL, Rocky, AlmaLinux, or Fedora-based systems:

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

Initialize a Conan profile:

```bash
~/.local/bin/conan profile detect --force
```

## Build System

The project now uses:

- Conan for dependency management
- CMake presets for consistent configuration
- Ninja as the preferred generator
- GitHub Actions for CI validation

Committed presets are provided in:

```text
CMakePresets.json
```

Available presets:

- `conan-release`
- `conan-debug`

## Recommended Local Build

The preferred local workflow uses the helper script:

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

### Release Build

```bash
conan install . --build=missing -s build_type=Release
cmake --preset=conan-release
cmake --build --preset=conan-release
```

### Debug Build

```bash
conan install . --build=missing -s build_type=Debug
cmake --preset=conan-debug
cmake --build --preset=conan-debug
```

## Run the Application

The executable location depends on the configured preset and build output.

Typical locations:

```text
build/Release/
build/Debug/
```

Find executables if needed:

```bash
find build -type f -executable
```

Example:

```bash
./build/Release/source/cursedmenu/cursedmenu
```

## Menu Files

### Preferred Format: JSON

JSON is the planned default menu definition format for new menus.

Goals of the JSON migration:

- Easier validation
- Easier tooling
- Better readability
- Structured parsing
- Better automated testing
- Extensible parser architecture

Planned JSON shape:

```json
{
  "version": 1,
  "menus": [
    {
      "id": "main",
      "title": "Main Menu",
      "items": [
        {
          "name": "System Info",
          "command": "uname -a"
        },
        {
          "name": "Exit",
          "action": "exit"
        }
      ]
    }
  ],
  "rootMenu": "main"
}
```

### Legacy Format: `.cmd`

The existing `.cmd` format remains supported during migration.

The modernization effort now includes:

- `JsonMenuParser`
- `LegacyCmdMenuParser`
- `MenuParser`
- `MenuParserFactory`
- `MenuParseResult`
- `MenuParseError`

The goal is for runtime code to operate on a shared menu model independent of file format.

## Run with a Menu File

Example using a legacy `.cmd` file:

```bash
./build/Release/source/cursedmenu/cursedmenu -m default.cmd
```

Future JSON example:

```bash
./build/Release/source/cursedmenu/cursedmenu -m default.json
```

## Validation / Check Mode

Use `-c` to validate or check menu definitions without running the interactive UI.

Legacy `.cmd` example:

```bash
./build/Release/source/cursedmenu/cursedmenu -m default.cmd -c
```

Future JSON example:

```bash
./build/Release/source/cursedmenu/cursedmenu -m default.json -c
```

## Help

```bash
./build/Release/source/cursedmenu/cursedmenu --help
```

## CI Workflow

GitHub Actions CI currently verifies:

- Conan dependency installation
- Conan/CMake preset configuration
- Project compilation

Future CI goals:

- automated tests
- sanitizer builds
- static analysis
- formatting validation
- coverage reporting

## Troubleshooting

### Conan Issues

Re-detect the Conan profile:

```bash
conan profile detect --force
```

Clear previous build artifacts:

```bash
./clean
```

### ncurses or terminal issues

Check:

```bash
echo "$TERM"
echo "$TERMINFO"
ls /usr/share/terminfo
```

The modernization effort is moving terminal handling into isolated runtime classes to improve startup diagnostics and exception safety.

### Preset Problems

Verify presets:

```bash
cmake --list-presets
```

## Developer Workflow

See:

```text
docs/developer-workflow.md
```

Related documents:

- `APP_PLAN.md`
- `docs/coding-standards.md`
- `docs/developer-workflow.md`

## Modernization Backlog

Current modernization workstreams include:

- documentation modernization
- coding standards and formatting
- JSON parser architecture
- ncurses runtime separation
- memory safety and exception handling
- CI and build tooling
- parser validation and migration tooling
- automated testing
- runtime UX improvements

## License

CursedMenu has historically been distributed under the GNU General Public License. Preserve existing license and copyright headers when modernizing source files.
