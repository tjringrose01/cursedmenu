from conan import ConanFile
from conan.tools.cmake import cmake_layout


class CursedMenuRecipe(ConanFile):
    settings = "os", "compiler", "build_type", "arch"
    generators = "CMakeDeps", "CMakeToolchain"

    def requirements(self):
        self.requires("ncurses/6.5")
        self.requires("rapidjson/1.1.0")

    def layout(self):
        cmake_layout(self)
