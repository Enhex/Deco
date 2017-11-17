#include <deco/set.h>
#include <deco/types/arithmetic.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>


int main()
{
	int i = 2;
	int i_ = 0;

	std::string name = "read name";

	// write file
	{
		deco::OutputStream_indent stream;

		deco::serialize(stream, deco::make_set(i));		
		deco::serialize(stream, deco::make_set("ignore name", i));
		deco::serialize(stream, deco::make_set(name, i));

		gs::serializer(stream, deco::make_set(i));
		gs::serializer(stream, deco::make_set("ignore name", i));
		gs::serializer(stream, deco::make_set(name, i));

		deco::serialize(stream, deco::begin_set());
		deco::serialize(stream, deco::begin_set("ignore name"));
		deco::serialize(stream, deco::begin_set(name));
		deco::serialize(stream, deco::end_set);
		deco::serialize(stream, deco::end_set);
		deco::serialize(stream, deco::end_set);

		gs::serializer(stream, deco::begin_set());
		gs::serializer(stream, deco::begin_set("ignore name"));
		gs::serializer(stream, deco::begin_set(name));
		gs::serializer(stream, deco::end_set);
		gs::serializer(stream, deco::end_set);
		gs::serializer(stream, deco::end_set);


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

		deco::serialize(stream, deco::make_set(i_));				assert(i_ == i);
		deco::serialize(stream, deco::make_set("ignore name", i_));	assert(i_ == i);
		deco::serialize(stream, deco::make_set(name, i_));			assert(i_ == i);

		gs::serializer(stream, deco::make_set(i_));					assert(i_ == i);
		gs::serializer(stream, deco::make_set("ignore name", i_));	assert(i_ == i);
		gs::serializer(stream, deco::make_set(name, i_));			assert(i_ == i);

		deco::serialize(stream, deco::begin_set());
		deco::serialize(stream, deco::begin_set("ignore name"));
		deco::serialize(stream, deco::begin_set(name));
		deco::serialize(stream, deco::end_set);
		deco::serialize(stream, deco::end_set);
		deco::serialize(stream, deco::end_set);

		gs::serializer(stream, deco::begin_set());
		gs::serializer(stream, deco::begin_set("ignore name"));
		gs::serializer(stream, deco::begin_set(name));
		gs::serializer(stream, deco::end_set);
		gs::serializer(stream, deco::end_set);
		gs::serializer(stream, deco::end_set);
	}
}