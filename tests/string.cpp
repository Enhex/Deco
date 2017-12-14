#include <deco/types/string.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>

int main()
{
	const std::string str_val =					"string";
	const std::string str_space =				"  string";
	const std::string str_tab =					"		string";
	const std::string str_spc_tab =				"		string";
	const std::string str_content_begin =		"' string";
	const std::string str_content_end =			"string'";
	const std::string str_structure =			"string:";
	const std::string str_structure_con_end =	"string:'";

	// write
	{
		deco::OutputStream_indent stream;
		
		gs::serializer(stream,
			str_val,
			str_space,
			str_tab,
			str_spc_tab,
			str_content_begin,
			str_content_end,
			str_structure,
			str_structure_con_end);

		std::ofstream os("out.deco", std::ios::binary);
		os << stream.str;
	}

	// read
	{
		auto file = std::ifstream("out.deco", std::ios::binary);
		std::string file_str{
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>() };

		std::cout << file_str;

		auto stream = deco::make_InputStream(file_str.cbegin());

		const auto serialize = [&stream](auto&& t) {
			deco::serialize(stream, t);
		};

		std::string str;
		serialize(str); assert(str == str_val);
		serialize(str); assert(str == str_space);
		serialize(str); assert(str == str_tab);
		serialize(str); assert(str == str_spc_tab);
		serialize(str); assert(str == str_content_begin);
		serialize(str); assert(str == str_content_end);
		serialize(str); assert(str == str_structure);
		serialize(str); assert(str == str_structure_con_end);
	}
}