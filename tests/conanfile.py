from conans import ConanFile

class install(ConanFile):
	settings = "os", "compiler", "build_type", "arch"
	requires = (
		"generic_serialization/master@enhex/stable",
		"rang/3.2",
		"boost/1.79.0",
		"fmt/8.1.1",
		"strong_type/master@enhex/stable"
	)
	generators = "premake"