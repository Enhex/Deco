#include <deco/NVP.h>
#include <deco/list.h>
#include <deco/types/arithmetic.h>
#include <deco/types/array.h>
#include <deco/types/deque.h>
#include <deco/types/forward_list.h>
#include <deco/types/list.h>
#include <deco/types/map.h>
#include <deco/types/multiline_string.h>
#include <deco/types/multimap.h>
#include <deco/types/multiset.h>
#include <deco/types/set.h>
#include <deco/types/string.h>
#include <deco/types/unescaped_string.h>
#include <deco/types/unordered_map.h>
#include <deco/types/unordered_multimap.h>
#include <deco/types/unordered_multiset.h>
#include <deco/types/unordered_set.h>
#include <deco/types/vector.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>

constexpr auto floating_test_value = 123.125;	// 0.125 shouldn't have floating point precision error

template<typename T, typename Stream>
void write_type(Stream& stream)
{
	if constexpr (std::is_floating_point_v<T>) {
		auto value = T(floating_test_value);
		gs::serializer(stream, value);
	}
	else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>) {
		auto value = std::numeric_limits<T>::min();
		gs::serializer(stream, value);
	}
	else {
		auto value = std::numeric_limits<T>::max();
		gs::serializer(stream, value);
	}
};

template<typename T, typename I>
void read_type(deco::InputStream<I>& stream)
{
	T value;
	gs::serializer(stream, value);

	if constexpr (std::is_floating_point_v<T>)
		assert(value == floating_test_value);
	else if constexpr (std::is_integral_v<T> && std::is_signed_v<T>)
		assert(value == std::numeric_limits<T>::min());
	else
		assert(value == std::numeric_limits<T>::max());
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
	const std::array<int, 5> arr_val{ el++,el++,el++,el++,el++ };
	const std::vector<int> v_val{ el++,el++,el++,el++,el++ };
	const std::set<int> set_val{ el++,el++,el++,el++,el++ };
	const std::deque<int> deq_val{ el++,el++,el++,el++,el++ };
	const std::forward_list<int> flist_val{ el++,el++,el++,el++,el++ };
	const std::list<int> list_val{ el++,el++,el++,el++,el++ };
	const std::unordered_set<int> uset_val{ el++,el++,el++,el++,el++ };
	const std::multiset<int> mset_val{ el++,el,el++,el,el++ };
	const std::unordered_multiset<int> umset_val{ el++,el++,el++,el++,el++ };
	const std::map<int, int> map_val{ { el++,el++ },{ el++,el++ },{ el++, el++ } };
	const std::multimap<int, int> mmap_val{ { el++,el++ },{ el,el },{ el++, el++ } };
	const std::unordered_map<int, int> umap_val{ { el++,el++ },{ el,el },{ el++, el++ } };
	const std::unordered_multimap<int, int> ummap_val{ {el++,el++}, {el,el}, {el++, el++} };

	// write
	{
		deco::OutputStream_indent stream;
		const auto serialize = [&stream](auto&& t) {
			deco::serialize(stream, t);
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

		serialize(deco::make_list("list", str_val));

		serialize(deco::make_NVP("nvp", str_val));

		serialize(deco::make_list("multiline_string", ml_str_val));

		stream.begin_list("integral");
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
		stream.end_list();

		stream.begin_list("floating point");
			write_type<float>(stream);
			write_type<double>(stream);
			write_type<long double>(stream);
		stream.end_list();

		serialize(deco::make_list("std::array", arr_val));
		serialize(deco::make_list("std::vector", v_val));
		serialize(deco::make_list("std::set", set_val));
		serialize(deco::make_list("std::deque", deq_val));
		serialize(deco::make_list("std::forward_list", flist_val));
		serialize(deco::make_list("std::list", list_val));
		serialize(deco::make_list("std::unordered_set", uset_val));
		serialize(deco::make_list("std::multiset", mset_val));
		serialize(deco::make_list("std::unordered_multiset", umset_val));
		serialize(deco::make_list("std::map", map_val));
		serialize(deco::make_list("std::multimap", mmap_val));
		serialize(deco::make_list("std::unordered_map", umap_val));
		serialize(deco::make_list("std::unordered_multimap", ummap_val));

		std::ofstream os("out.deco", std::ios::binary);
		os << stream.str;
	}

	// read
	{
		auto file = std::ifstream("out.deco", std::ios::binary);
		std::string file_str{
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>()};

		std::cout << file_str;

		auto stream = deco::make_InputStream(file_str.cbegin());
		const auto serialize = [&stream](auto&& t) {
			deco::serialize(stream, t);
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

		serialize(deco::make_list("list", str)); assert(str == str_val);

		serialize(deco::make_NVP("nvp", str)); assert(str == str_val);

		deco::multiline_string ml_str; // dummy
		serialize(deco::make_list("multiline_string", ml_str)); assert(ml_str == ml_str_val);

		serialize(str); assert(str == "integral");			// list name
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
		stream.parse_entry();		// list end

		serialize(str); assert(str == "floating point");	// list name
			read_type<float>(stream);
			read_type<double>(stream);
			read_type<long double>(stream);
		stream.parse_entry();		// list end

		std::array<int, 5> arr;
		serialize(deco::make_list("std::array", arr)); assert(arr == arr_val);
		std::vector<int> v;
		serialize(deco::make_list("std::vector", v)); assert(v == v_val);
		std::set<int> set;
		serialize(deco::make_list("std::set", set)); assert(set == set_val);
		std::deque<int> deq;
		serialize(deco::make_list("std::deque", deq)); assert(deq == deq_val);
		std::forward_list<int> flist;
		serialize(deco::make_list("std::forward_list", flist)); assert(flist == flist_val);
		std::list<int> li;
		serialize(deco::make_list("std::list", li)); assert(li == list_val);
		std::unordered_set<int> uset;
		serialize(deco::make_list("std::unordered_set", uset)); assert(uset == uset_val);
		std::multiset<int> mset;
		serialize(deco::make_list("std::multiset", mset)); assert(mset == mset_val);
		std::unordered_multiset<int> umset;
		serialize(deco::make_list("std::unordered_multiset", umset)); assert(umset == umset_val);
		std::map<int, int> map_;
		serialize(deco::make_list("std::map", map_)); assert(map_ == map_val);
		std::multimap<int, int> mmap;
		serialize(deco::make_list("std::multimap", mmap)); assert(mmap == mmap_val);
		std::unordered_map<int, int> umap;
		serialize(deco::make_list("std::unordered_map", umap)); assert(umap == umap_val);
		std::unordered_multimap<int, int> ummap;
		serialize(deco::make_list("std::unordered_multimap", ummap)); assert(ummap == ummap_val);
	}
}