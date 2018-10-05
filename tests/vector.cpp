#include <deco/list.h>
#include <deco/types/vector.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>

int main()
{
	const std::vector<std::string> val{ "1", "2", "3", "4", "5" };
	const std::vector<std::vector<std::string>> val2{ {"1", "2"}, {"3", "4", "5"} };

	// write
	{
		deco::OutputStream_indent stream;
		
		deco::serialize(stream, deco::make_list("std::vector", val));
		deco::serialize(stream, deco::make_list("std::vector2", val2));

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

		std::vector<std::string> in_val;
		deco::serialize(stream, deco::make_list("std::vector", in_val)); assert(in_val == val);
		std::vector<std::vector<std::string>> in_val2;
		deco::serialize(stream, deco::make_list("std::vector2", in_val2)); assert(in_val2 == val2);
	}
}