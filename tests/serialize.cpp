#include <deco/NVP.h>
#include <deco/escaped_string.h>
#include <deco/multiline_string.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

constexpr auto floating_test_value = 123.125;

template<typename T>
void write_type(deco::OutputStream& stream) {
	if constexpr(is_floating_point_v<T>) {
		auto value = T(floating_test_value);
		gs::serialize(stream, value);
	}
	else if (is_integral_v<T> && is_signed_v<T>) {
		auto value = numeric_limits<T>::min();
		gs::serialize(stream, value);
	}
	else {
		auto value = numeric_limits<T>::max();
		gs::serialize(stream, value);
	}
};

template<typename T>
void read_type(deco::InputStream& stream) {
	T value;
	gs::serialize(stream, value);
	if constexpr(is_floating_point_v<T>)
		assert(value == floating_test_value);
	else if(is_integral_v<T> && is_signed_v<T>)
		assert(value == numeric_limits<T>::min());
	else
		assert(value == numeric_limits<T>::max());
};


int main()
{
	const string str_val = "string";
	const deco::escaped_string esc_str_space =				"  string";
	const deco::escaped_string esc_str_tab =				"		string";
	const deco::escaped_string esc_str_spc_tab =			"		string";
	const deco::escaped_string esc_str_content_begin =		"' string";
	const deco::escaped_string esc_str_content_end =		"string'";
	const deco::escaped_string esc_str_structure =			"string:";
	const deco::escaped_string esc_str_structure_con_end = "string:'";

	const deco::multiline_string ml_str_val = "multi\nline\nstring";

	vector<int> v_val{1,2,3,4,5,6,7,8};

	// write
	{
		deco::OutputStream_Indent stream;

		gs::serialize(stream, str_val);

		gs::serialize(stream, esc_str_space);
		gs::serialize(stream, esc_str_tab);
		gs::serialize(stream, esc_str_spc_tab);
		gs::serialize(stream, esc_str_content_begin);
		gs::serialize(stream, esc_str_content_end);
		gs::serialize(stream, esc_str_structure);
		gs::serialize(stream, esc_str_structure_con_end);

		gs::serialize(stream, deco::make_NVP("multiline_string", ml_str_val));
		//gs::serialize(stream, ml_str_val);


		stream.begin_set("integral");
			write_type<char>(stream);
			write_type<unsigned char>(stream);
			write_type<short>(stream);
			write_type<unsigned short>(stream);
			write_type<int>(stream);
			write_type<unsigned int>(stream);
			write_type<long>(stream);
			write_type<unsigned long>(stream);
			write_type<long long>(stream);
			write_type<unsigned long long>(stream);
		stream.end_set();

		stream.begin_set("floating point");
			write_type<float>(stream);
			write_type<double>(stream);
			write_type<long double>(stream);
		stream.end_set();

		gs::serialize(stream, deco::make_NVP("vector", v_val));

		ofstream os("out.deco", ios::binary);
		os << stream.str;
	}

	// read
	{
		string file_str{
			istreambuf_iterator<char>(ifstream("out.deco", ios::binary)),
			istreambuf_iterator<char>()};

		cout << file_str;

		deco::InputStream stream(file_str.cbegin());

		std::string str; // dummy
		vector<int> v;

		gs::serialize(stream, str); assert(str == str_val);

		deco::escaped_string esc_str; // dummy
		gs::serialize(stream, esc_str); assert(esc_str == esc_str_space);
		gs::serialize(stream, esc_str); assert(esc_str == esc_str_tab);
		gs::serialize(stream, esc_str); assert(esc_str == esc_str_spc_tab);
		gs::serialize(stream, esc_str); assert(esc_str == esc_str_content_begin);
		gs::serialize(stream, esc_str); assert(esc_str == esc_str_content_end);
		gs::serialize(stream, esc_str); assert(esc_str == esc_str_structure);
		gs::serialize(stream, esc_str); assert(esc_str == esc_str_structure_con_end);

		deco::multiline_string ml_str; // dummy
		gs::serialize(stream, deco::make_NVP("multiline_string", ml_str)); assert(ml_str == ml_str_val);

		gs::serialize(stream, str); assert(str == "integral");			// set name
			read_type<char>(stream);
			read_type<unsigned char>(stream);
			read_type<short>(stream);
			read_type<unsigned short>(stream);
			read_type<int>(stream);
			read_type<unsigned int>(stream);
			read_type<long>(stream);
			read_type<unsigned long>(stream);
			read_type<long long>(stream);
			read_type<unsigned long long>(stream);
		stream.parse_entry();		// set end

		gs::serialize(stream, str); assert(str == "floating point");	// set name
			read_type<float>(stream);
			read_type<double>(stream);
			read_type<long double>(stream);
		stream.parse_entry();		// set end

		gs::serialize(stream, deco::make_NVP("vector", v));
		assert(v == v_val);
	}
}