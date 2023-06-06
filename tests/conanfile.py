from conans import ConanFile

class install(ConanFile):
	settings = "os", "compiler", "build_type", "arch"
	requires = (
		"generic_serialization/master@enhex/stable",
		"rang/3.2",
		"boost/1.82.0",
		"fmt/10.0.0",
		"strong_type/master@enhex/stable"
	)
	generators = "premake"