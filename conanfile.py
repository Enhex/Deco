from conans import ConanFile, tools


class DecoConan(ConanFile):
	name = "deco"
	version = "master"
	license = "Apache-2.0 WITH LLVM-exception"
	url = "https://github.com/Enhex/Deco"
	description = "Delimiter Collision Free Format"
	homepage = "https://github.com/Enhex/Deco"
	requires = (
		"generic_serialization/master@enhex/stable",
		"rang/3.2",
		"boost/1.78.0",
		"strong_type/master@enhex/stable"
	)
	# No settings/options are necessary, this is header only
	exports_sources = "include/*"

	def package(self):
		self.copy("*.h")

	def package_id(self):
		self.info.header_only()
