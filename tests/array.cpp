#include <deco/set.h>
#include <deco/types/array.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>

int main()
{
	const std::array<std::string, 5> val{ "1", "2", "3", "4", "5", };

	// write
	{
		deco::OutputStream_indent stream;
		const auto serialize = [&stream](auto&& t) {
			deco::serialize(stream, t);
		};

		serialize(deco::make_set("std::array", val));

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

		std::array<std::string, 5> in_val;
		deco::serialize(stream, deco::make_set("std::array", in_val)); assert(in_val == val);
	}
}