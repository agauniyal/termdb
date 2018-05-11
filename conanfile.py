from conans import ConanFile, Meson


class TermDbConan(ConanFile):
    name = "termdb"
    version = "0.10.0"
    license = "MIT"
    url = "https://github.com/agauniyal/termdb"
    description = "Terminfo parser for modern c++"
    generators = "pkg_config"
    requires = "optional-lite/2.3.0@nonstd-lite/stable", "variant/1.3.0@rang/stable"
    build_requires = "doctest/1.2.6@bincrafters/stable"

    def build(self):
        meson = Meson(self)
        meson.configure()
        meson.build()

    def package(self):
        self.copy("*.hpp")

    def package_id(self):
        self.info.header_only()
