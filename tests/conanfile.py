from conan import ConanFile

class install(ConanFile):
	settings = "os", "compiler", "build_type", "arch"
	requires = (
		"generic_serialization/master",
		"rang/3.2",
		"boost/1.86.0",
		"fmt/11.0.2",
		"strong_type/master"
	)
	generators = "PremakeDeps"