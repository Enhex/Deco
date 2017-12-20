#include <deco/NVP.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>


int main()
{
	const std::string str_val =					"string";
	const std::string str_empty =				"";
	const std::string str_space =				"  string";
	const std::string str_tab =					"		string";
	const std::string str_spc_tab =				"		string";
	const std::string str_content_begin =		"' string";
	const std::string str_content_end =			"string'";
	const std::string str_content_end2 =		"string''";
	const std::string str_structure =			"string:";
	const std::string str_structure_con_end =	"string:'";

	const int i = 2;

	// write file
	{
		deco::OutputStream_indent stream;
		
		gs::serializer(stream, deco::make_NVP("ignored name", i));
		gs::serializer(stream, deco::make_NVP(str_val, i));

		deco::serialize(stream, deco::make_NVP("ignored name", "value"));
		
		deco::serialize(stream, deco::make_NVP(str_val, str_val));
		deco::serialize(stream, deco::make_NVP(str_empty, str_empty));
		deco::serialize(stream, deco::make_NVP(str_space, str_space));
		deco::serialize(stream, deco::make_NVP(str_tab, str_tab));
		deco::serialize(stream, deco::make_NVP(str_spc_tab, str_spc_tab));
		deco::serialize(stream, deco::make_NVP(str_content_begin, str_content_begin));
		deco::serialize(stream, deco::make_NVP(str_content_end, str_content_end));
		deco::serialize(stream, deco::make_NVP(str_content_end2, str_content_end2));
		// name can't contain structure delimiter
		deco::serialize(stream, deco::make_NVP(str_val, str_structure));
		deco::serialize(stream, deco::make_NVP(str_val, str_structure_con_end));
		
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

		std::string in_name, in_str;
		int in_i;

		gs::serializer(stream, deco::make_NVP("ignored name", in_i));		assert(in_i == i);
		gs::serializer(stream, deco::make_NVP(in_name, in_i));				assert(in_name == str_val); assert(in_i == i);

		deco::serialize(stream, deco::make_NVP("ignored name", in_str));	assert(in_str == "value");

		deco::serialize(stream, deco::make_NVP(in_name, in_str));			assert(in_name == str_val); assert(in_str == str_val);
		deco::serialize(stream, deco::make_NVP(in_name, in_str));			assert(in_name == str_empty); assert(in_str == str_empty);
		deco::serialize(stream, deco::make_NVP(in_name, in_str));			assert(in_name == str_space); assert(in_str == str_space);
		deco::serialize(stream, deco::make_NVP(in_name, in_str));			assert(in_name == str_tab); assert(in_str == str_tab);
		deco::serialize(stream, deco::make_NVP(in_name, in_str));			assert(in_name == str_spc_tab); assert(in_str == str_spc_tab);
		deco::serialize(stream, deco::make_NVP(in_name, in_str));			assert(in_name == str_content_begin); assert(in_str == str_content_begin);
		deco::serialize(stream, deco::make_NVP(in_name, in_str));			assert(in_name == str_content_end); assert(in_str == str_content_end);
		deco::serialize(stream, deco::make_NVP(in_name, in_str));			assert(in_name == str_content_end2); assert(in_str == str_content_end2);
		deco::serialize(stream, deco::make_NVP(in_name, in_str));			assert(in_name == str_val); assert(in_str == str_structure);
		deco::serialize(stream, deco::make_NVP(in_name, in_str));			assert(in_name == str_val); assert(in_str == str_structure_con_end);
	}
}