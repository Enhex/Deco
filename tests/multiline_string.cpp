#include <deco/set.h>
#include <deco/types/multiline_string.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>

int main()
{
	const deco::multiline_string ml_str_val{ "multi\nline\nstring" };

	// write
	{
		deco::OutputStream_indent stream;
		
		deco::serialize(stream, deco::make_set("multiline_string", ml_str_val));

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

		deco::multiline_string ml_str;
		serialize(deco::make_set("multiline_string", ml_str)); assert(ml_str == ml_str_val);
	}
}