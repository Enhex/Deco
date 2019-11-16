#include <deco/list.h>
#include <deco/file.h>
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
		deco::write_file file("out.deco");
		auto& stream = file.stream;

		deco::serialize(stream, deco::make_list("std::array", val));
	}

	// read
	{
		auto file = deco::read_file("out.deco");
		auto& stream = file.stream;

		std::cout << file.str;

		std::array<std::string, 5> in_val;
		deco::serialize(stream, deco::make_list("std::array", in_val)); assert(in_val == val);
	}
}