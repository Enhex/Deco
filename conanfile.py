from conan import ConanFile, tools
from conan.tools.files import copy


class DecoConan(ConanFile):
	name = "deco"
	version = "master"
	license = "Apache-2.0 WITH LLVM-exception"
	url = "https://github.com/Enhex/Deco"
	description = "Delimiter Collision Free Format"
	homepage = "https://github.com/Enhex/Deco"
	# No settings/options are necessary, this is header only
	exports_sources = "include/*"
	package_type = "header-library"
	requires = (
		"generic_serialization/master",
		"rang/3.2",
		"fmt/11.2.0",
		"strong_type/master"
	)

	def requirements(self):
		self.requires("fast_float/8.0.0", transitive_headers=True)

	def package(self):
		copy(self, "*.h", self.source_folder, self.package_folder)

	def package_id(self):
		self.info.clear()