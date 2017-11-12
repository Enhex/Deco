#include <deco/NVP.h>
#include <deco/set.h>
#include <deco/types/arithmetic.h>
#include <deco/types/deque.h>
#include <deco/types/forward_list.h>
#include <deco/types/list.h>
#include <deco/types/map.h>
#include <deco/types/multiline_string.h>
#include <deco/types/multiset.h>
#include <deco/types/set.h>
#include <deco/types/string.h>
#include <deco/types/unescaped_string.h>
#include <deco/types/unordered_multiset.h>
#include <deco/types/unordered_set.h>
#include <deco/types/vector.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>

using namespace std;

constexpr auto floating_test_value = 123.125;	// 0.125 shouldn't have floating point precision error

template<typename T, typename Stream>
void write_type(Stream& stream)
{
	if constexpr(is_floating_point_v<T>) {
		auto value = T(floating_test_value);
		gs::serializer(stream, value);
	}
	else if (is_integral_v<T> && is_signed_v<T>) {
		auto value = numeric_limits<T>::min();
		gs::serializer(stream, value);
	}
	else {
		auto value = numeric_limits<T>::max();
		gs::serializer(stream, value);
	}
};

template<typename T, typename I>
void read_type(deco::InputStream<I>& stream) {
	T value;
	gs::serializer(stream, value);
	if constexpr(is_floating_point_v<T>)
		assert(value == floating_test_value);
	else if(is_integral_v<T> && is_signed_v<T>)
		assert(value == numeric_limits<T>::min());
	else
		assert(value == numeric_limits<T>::max());
};


int main()
{
	const deco::unescaped_string unesc_str_val{ "string" };
	const std::string str_val =					"string";
	const std::string str_space =				"  string";
	const std::string str_tab =					"		string";
	const std::string str_spc_tab =				"		string";
	const std::string str_content_begin =		"' string";
	const std::string str_content_end =			"string'";
	const std::string str_structure =			"string:";
	const std::string str_structure_con_end =	"string:'";

	const deco::multiline_string ml_str_val{ "multi\nline\nstring" };

	int el = 0;	// element value
	const std::vector<int> v_val{ el++,el++,el++,el++,el++ };
	const std::set<int> set_val{ el++,el++,el++,el++,el++ };
	const std::deque<int> deq_val{ el++,el++,el++,el++,el++ };
	const std::forward_list<int> flist_val{ el++,el++,el++,el++,el++ };
	const std::list<int> list_val{ el++,el++,el++,el++,el++ };
	const std::unordered_set<int> uset_val{ el++,el++,el++,el++,el++ };
	const std::multiset<int> mset_val{ el++,el++,el++,el++,el++ };
	const std::unordered_multiset<int> umset_val{ el++,el++,el++,el++,el++ };
	const std::map<int, int> map_val{ {el++,el++}, {el++,el++}, {el++, el++} };

	// write
	{
		deco::OutputStream_indent stream;
		const auto serialize = [&stream](auto&& t) {
			gs::serializer(stream, t);
		};

		serialize(unesc_str_val);
		
		gs::serializer(stream,
			str_space,
			str_tab,
			str_spc_tab,
			str_content_begin,
			str_content_end,
			str_structure,
			str_structure_con_end);

		serialize(deco::make_set("set", str_val));

		serialize(deco::make_NVP("nvp", str_val));

		serialize(deco::make_set("multiline_string", ml_str_val));

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

		serialize(deco::make_set("std::vector", v_val));
		serialize(deco::make_set("std::set", set_val));
		serialize(deco::make_set("std::deque", deq_val));
		serialize(deco::make_set("std::forward_list", flist_val));
		serialize(deco::make_set("std::list", list_val));
		serialize(deco::make_set("std::unordered_set", uset_val));
		serialize(deco::make_set("std::multiset", mset_val));
		serialize(deco::make_set("std::unordered_multiset", umset_val));
		serialize(deco::make_set("std::map", map_val));

		ofstream os("out.deco", ios::binary);
		os << stream.str;
	}

	// read
	{
		auto file = ifstream("out.deco", ios::binary);
		string file_str{
			istreambuf_iterator<char>(file),
			istreambuf_iterator<char>()};

		cout << file_str;

		auto stream = deco::make_InputStream(file_str.cbegin());
		const auto serialize = [&stream](auto&& t) {
			gs::serializer(stream, t);
		};

		deco::unescaped_string unesc_str; // dummy
		gs::serializer(stream, unesc_str); assert(unesc_str == unesc_str_val);
		
		std::string str; // dummy
		serialize(str); assert(str == str_space);
		serialize(str); assert(str == str_tab);
		serialize(str); assert(str == str_spc_tab);
		serialize(str); assert(str == str_content_begin);
		serialize(str); assert(str == str_content_end);
		serialize(str); assert(str == str_structure);
		serialize(str); assert(str == str_structure_con_end);

		serialize(deco::make_set("set", str)); assert(str == str_val);
		
		serialize(deco::make_NVP("nvp", str)); assert(str == str_val);

		deco::multiline_string ml_str; // dummy
		serialize(deco::make_set("multiline_string", ml_str)); assert(ml_str == ml_str_val);

		serialize(str); assert(str == "integral");			// set name
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

		serialize(str); assert(str == "floating point");	// set name
			read_type<float>(stream);
			read_type<double>(stream);
			read_type<long double>(stream);
		stream.parse_entry();		// set end

		std::vector<int> v;
		serialize(deco::make_set("std::vector", v)); assert(v == v_val);
		std::set<int> set;
		serialize(deco::make_set("std::set", set)); assert(set == set_val);
		std::deque<int> deq;
		serialize(deco::make_set("std::deque", deq)); assert(deq == deq_val);
		std::forward_list<int> flist;
		serialize(deco::make_set("std::forward_list", flist)); assert(flist == flist_val);
		std::list<int> li;
		serialize(deco::make_set("std::list", li)); assert(li == list_val);
		std::unordered_set<int> uset;
		serialize(deco::make_set("std::unordered_set", uset)); assert(uset == uset_val);
		std::multiset<int> mset;
		serialize(deco::make_set("std::multiset", mset)); assert(mset == mset_val);
		std::unordered_multiset<int> umset;
		serialize(deco::make_set("std::unordered_multiset", umset)); assert(umset == umset_val);
		std::map<int, int> map_;
		serialize(deco::make_set("std::map", map_)); assert(map_ == map_val);
	}
}