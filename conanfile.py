from conans import ConanFile, CMake, tools


class DecoConan(ConanFile):
	name = "Deco"
	version = "master"
	license = "MIT"
	description = "Delimiter Collision Free Format"
	requires = (
		"generic_serialization/master@enhex/stable",
		"rang/master@enhex/stable",
		"Boost/1.66.0@enhex/testing",
		"strong_type/master@enhex/stable"
	)
	default_options = "Boost:header_only=True"
	# No settings/options are necessary, this is header only
	exports_sources = "include/*"

	def package(self):
		self.copy("*.h")