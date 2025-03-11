from conan import ConanFile

class install(ConanFile):
	settings = "os", "compiler", "build_type", "arch"
	requires = (
		"generic_serialization/master",
		"rang/3.2",
		"fmt/11.1.4",
		"strong_type/master"
	)

	def requirements(self):
		self.requires("fast_float/8.0.0", transitive_headers=True)

	generators = "PremakeDeps"