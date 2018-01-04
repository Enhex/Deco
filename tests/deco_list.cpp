#include <deco/list.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>


int main()
{
	std::string i = "2";
	std::string i_ = "0";

	std::string name = "read name";

	// write file
	{
		deco::OutputStream_indent stream;

		deco::serialize(stream, deco::make_list(i));		
		deco::serialize(stream, deco::make_list("ignore name", i));
		deco::serialize(stream, deco::make_list(name, i));

		gs::serializer(stream, deco::make_list(i));
		gs::serializer(stream, deco::make_list("ignore name", i));
		gs::serializer(stream, deco::make_list(name, i));

		deco::serialize(stream, deco::begin_list());
		deco::serialize(stream, deco::begin_list("ignore name"));
		deco::serialize(stream, deco::begin_list(name));
		deco::serialize(stream, deco::end_list);
		deco::serialize(stream, deco::end_list);
		deco::serialize(stream, deco::end_list);

		gs::serializer(stream, deco::begin_list());
		gs::serializer(stream, deco::begin_list("ignore name"));
		gs::serializer(stream, deco::begin_list(name));
		gs::serializer(stream, deco::end_list);
		gs::serializer(stream, deco::end_list);
		gs::serializer(stream, deco::end_list);


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

		deco::serialize(stream, deco::make_list(i_));				assert(i_ == i);
		deco::serialize(stream, deco::make_list("ignore name", i_));assert(i_ == i);
		deco::serialize(stream, deco::make_list(name, i_));			assert(i_ == i);

		gs::serializer(stream, deco::make_list(i_));				assert(i_ == i);
		gs::serializer(stream, deco::make_list("ignore name", i_));	assert(i_ == i);
		gs::serializer(stream, deco::make_list(name, i_));			assert(i_ == i);

		deco::serialize(stream, deco::begin_list());
		deco::serialize(stream, deco::begin_list("ignore name"));
		deco::serialize(stream, deco::begin_list(name));
		deco::serialize(stream, deco::end_list);
		deco::serialize(stream, deco::end_list);
		deco::serialize(stream, deco::end_list);

		gs::serializer(stream, deco::begin_list());
		gs::serializer(stream, deco::begin_list("ignore name"));
		gs::serializer(stream, deco::begin_list(name));
		gs::serializer(stream, deco::end_list);
		gs::serializer(stream, deco::end_list);
		gs::serializer(stream, deco::end_list);
	}
}