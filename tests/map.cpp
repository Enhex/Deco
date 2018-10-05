#include <deco/list.h>
#include <deco/types/map.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>

int main()
{
	using T1 = std::map<std::string, std::string>;
	const T1 val{ {"a", "1"}, {"b", "2"}, {"c", "3"}, {"d", "4"}, {"e", "5"} };
	using T2 = std::map<std::string, std::map<std::string, std::string>>;
	const T2 val2{
		{ "a", { { "1", "1" } } },
		{ "b", { { "2", "2" } } }
	};

	// write
	{
		deco::OutputStream_indent stream;

		deco::serialize(stream, deco::make_list("std::map", val));
		deco::serialize(stream, deco::make_list("std::map2", val2));

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

		T1 in_val;
		deco::serialize(stream, deco::make_list("std::map", in_val)); assert(in_val == val);
		T2 in_val2;
		deco::serialize(stream, deco::make_list("std::map2", in_val2)); assert(in_val2 == val2);
	}
}