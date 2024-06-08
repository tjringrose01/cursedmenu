# Requirements

Conan > v2.3.0
CMake > v3.26

# To Build

To build cursedmenu follow these steps. 

### Download and compile dependencies

```bash
conan install . --build=missing
```

### Build out the CMake files and environment

```bash
cmake . --preset=conan-release
```

### Setup pathing for conan 

```bash
source build/Release/generators/conanbuild.sh
```

### Perform the build

```bash
cmake --build . "${PRESET}"
```

### Remove pathing environment

```bash
source build/${RELEASE_FOLDER}/generators/deactivate_conanbuild.sh
```
