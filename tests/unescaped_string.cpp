#include <deco/types/unescaped_string.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>

int main()
{
	// whitespace will be written without escaping, thus skipped when reading
	const deco::unescaped_string str_val	{ "string" };
	const deco::unescaped_string str_space	{ std::string("  ") + str_val };
	const deco::unescaped_string str_tab	{ std::string("		") + str_val };

	// write
	{
		deco::OutputStream_indent stream;
		
		gs::serializer(stream,
			str_val,
			str_space,
			str_tab);

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

		deco::unescaped_string str;
		deco::serialize(stream, str); assert(str == str_val);
		deco::serialize(stream, str); assert(str == str_val);
		deco::serialize(stream, str); assert(str == str_val);
	}
}