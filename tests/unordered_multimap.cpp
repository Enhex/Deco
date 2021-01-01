#include <deco/list.h>
#include <deco/types/unordered_multimap.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>

int main()
{
	const std::unordered_multimap<std::string, std::string> val{ {"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}, {"e", "5"} };

	// write
	{
		deco::OutputStream_indent stream;

		deco::serialize(stream, deco::make_list("std::unordered_multimap", val));

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

		std::unordered_multimap<std::string, std::string> in_val;
		deco::serialize(stream, deco::make_list("std::unordered_multimap", in_val)); assert(in_val == val);
	}
}