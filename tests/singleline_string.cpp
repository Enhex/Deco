#include <deco/list.h>
#include <deco/types/map.h>
#include <deco/types/singleline_string.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>

int main()
{
	const std::map<deco::singleline_string, deco::singleline_string> val {
		{ deco::singleline_string("a"), deco::singleline_string("1") },
		{ deco::singleline_string("b"), deco::singleline_string("2") },
		{ deco::singleline_string("c"), deco::singleline_string("3") }
	};

	// write
	{
		deco::OutputStream_indent stream;

		deco::serialize(stream, deco::make_list("singleline_string", val));

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

		std::map<deco::singleline_string, deco::singleline_string> in_val;
		deco::serialize(stream, deco::make_list(in_val)); assert(in_val == val);
	}
}