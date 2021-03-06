#include <deco/list.h>
#include <deco/types/forward_list.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>

int main()
{
	const std::forward_list<std::string> val{ "1", "2", "3", "4", "5", };

	// write
	{
		deco::OutputStream_indent stream;

		deco::serialize(stream, deco::make_list("std::forward_list", val));

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

		std::forward_list<std::string> in_val;
		deco::serialize(stream, deco::make_list("std::forward_list", in_val)); assert(in_val == val);
	}
}