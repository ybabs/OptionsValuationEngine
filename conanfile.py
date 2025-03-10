from conans import ConanFile, CMake

class OptionPricingEngineConan(ConanFile):
    name = "ValuationEngine"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    requires = "boost/1.79.0"
    generators = "cmake", "cmake_find_package"
    exports_sources = "src/*", "inc/*", "CMakeLists.txt"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        self.copy("*.h", dst="include", src="inc")
        self.copy("*.hpp", dst="include", src="inc")
        self.copy("*.h", dst="include", src="src")
        self.copy("*.hpp", dst="include", src="src")
        self.copy("*ValuationEngine*", dst="bin", keep_path=False)

    def package_info(self):
        self.cpp_info.libs = ["ValuationEngine"]