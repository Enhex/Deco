#include <deco/NVP.h>
#include <deco/types/arithmetic.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>


int main()
{
	int i = 2;
	int i_ = 0;

	std::string name = "	 read name";	// test whitespace skipping too

	// write file
	{
		deco::OutputStream_indent stream;

		deco::serialize(stream, deco::make_NVP("ignore name", i));		
		deco::serialize(stream, deco::make_NVP(name, i));

		gs::serializer(stream, deco::make_NVP("ignore name", i));
		gs::serializer(stream, deco::make_NVP(name, i));

		std::ofstream os("out.deco", std::ios::binary);
		os << stream.str;
	}

	// read file
	{
		auto file = std::ifstream("out.deco", std::ios::binary);
		std::string file_str{
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>()};

		auto stream = deco::make_InputStream(file_str.cbegin());

		deco::serialize(stream, deco::make_NVP("ignore name", i_));	assert(i_ == i);
		deco::serialize(stream, deco::make_NVP(name, i_));			assert(i_ == i);

		gs::serializer(stream, deco::make_NVP("ignore name", i_));	assert(i_ == i);
		gs::serializer(stream, deco::make_NVP(name, i_));			assert(i_ == i);
	}
}