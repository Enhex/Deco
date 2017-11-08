#include <deco/set.h>
#include <deco/types/arithmetic.h>
#include <deco/types/multiline_string.h>
#include <deco/types/string.h>
#include <deco/types/unescaped_string.h>
#include <deco/types/vector.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

constexpr auto floating_test_value = 123.125;

template<typename T, typename Stream>
void write_type(gs::Serializer<Stream>& serializer)
{
	if constexpr(is_floating_point_v<T>) {
		auto value = T(floating_test_value);
		serializer(value);
	}
	else if (is_integral_v<T> && is_signed_v<T>) {
		auto value = numeric_limits<T>::min();
		serializer(value);
	}
	else {
		auto value = numeric_limits<T>::max();
		serializer(value);
	}
};

template<typename T>
void read_type(gs::Serializer<deco::InputStream&>& serializer) {
	T value;
	serializer(value);
	if constexpr(is_floating_point_v<T>)
		assert(value == floating_test_value);
	else if(is_integral_v<T> && is_signed_v<T>)
		assert(value == numeric_limits<T>::min());
	else
		assert(value == numeric_limits<T>::max());
};


int main()
{
	const deco::unescaped_string unesc_str_val ="string";
	const std::string str_val =					"string";
	const std::string str_space =				"  string";
	const std::string str_tab =					"		string";
	const std::string str_spc_tab =				"		string";
	const std::string str_content_begin =		"' string";
	const std::string str_content_end =			"string'";
	const std::string str_structure =			"string:";
	const std::string str_structure_con_end =	"string:'";

	const deco::multiline_string ml_str_val = "multi\nline\nstring";

	vector<int> v_val{1,2,3,4,5,6,7,8};
	
	// write
	{
		deco::OutputStream_Indent stream;
		auto serializer = gs::make_serializer(stream);

		serializer(unesc_str_val);
		
		serializer(
			str_space,
			str_tab,
			str_spc_tab,
			str_content_begin,
			str_content_end,
			str_structure,
			str_structure_con_end);

		serializer(deco::make_set("set", str_val));

		serializer(deco::make_set("multiline_string", ml_str_val));

		stream.begin_set("integral");
			write_type<char>(serializer);
			write_type<unsigned char>(serializer);
			write_type<short>(serializer);
			write_type<unsigned short>(serializer);
			write_type<int>(serializer);
			write_type<unsigned int>(serializer);
			write_type<long>(serializer);
			write_type<unsigned long>(serializer);
			write_type<long long>(serializer);
			write_type<unsigned long long>(serializer);
		stream.end_set();
		
		stream.begin_set("floating point");
			write_type<float>(serializer);
			write_type<double>(serializer);
			write_type<long double>(serializer);
		stream.end_set();

		serializer(deco::make_set("vector", v_val));

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
		auto serializer = gs::make_serializer(stream);

		deco::unescaped_string unesc_str; // dummy
		serializer(unesc_str); assert(unesc_str == unesc_str_val);
		
		std::string str; // dummy
		serializer(str); assert(str == str_space);
		serializer(str); assert(str == str_tab);
		serializer(str); assert(str == str_spc_tab);
		serializer(str); assert(str == str_content_begin);
		serializer(str); assert(str == str_content_end);
		serializer(str); assert(str == str_structure);
		serializer(str); assert(str == str_structure_con_end);

		serializer(deco::make_set("set", str)); assert(str == str_val);

		deco::multiline_string ml_str; // dummy
		serializer(deco::make_set("multiline_string", ml_str)); assert(ml_str == ml_str_val);

		serializer(str); assert(str == "integral");			// set name
			read_type<char>(serializer);
			read_type<unsigned char>(serializer);
			read_type<short>(serializer);
			read_type<unsigned short>(serializer);
			read_type<int>(serializer);
			read_type<unsigned int>(serializer);
			read_type<long>(serializer);
			read_type<unsigned long>(serializer);
			read_type<long long>(serializer);
			read_type<unsigned long long>(serializer);
		stream.parse_entry();		// set end

		serializer(str); assert(str == "floating point");	// set name
			read_type<float>(serializer);
			read_type<double>(serializer);
			read_type<long double>(serializer);
		stream.parse_entry();		// set end

		vector<int> v;
		serializer(deco::make_set("vector", v));
		assert(v == v_val);
	}
}