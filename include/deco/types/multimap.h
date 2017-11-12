#ifndef deco_std_multimap_h
#define deco_std_multimap_h

#include "../set_container.h"
#include <map>

namespace deco
{
	template<typename... Args>
	struct is_set_container<std::multimap<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
	void write_element_sets(Stream& stream, const std::multimap<Args...>& value)
	{
		for (auto& e : value) {
			stream.begin_set("key");
				gs::serialize(stream, e.first);
			stream.end_set();
			stream.begin_set("value");
				gs::serialize(stream, e.second);
			stream.end_set();
		}
	}

	template<typename Stream, typename... Args> constexpr
	void read_element_sets(Stream& stream, std::multimap<Args...>& value)
	{
		typename std::multimap<Args...>::key_type key_input;
		typename std::multimap<Args...>::mapped_type mapped_input;

		while (!stream.peek_set_end()) {
			gs::serialize(stream, skip);		// skip set name
			gs::serialize(stream, key_input);	// read value
			stream.parse_entry();				// skip set end

			gs::serialize(stream, skip);		// skip set name
			gs::serialize(stream, mapped_input);// read value
			stream.parse_entry();				// skip set end

			value.emplace(std::make_pair(key_input, mapped_input));
		}
	}
}

#endif//guard
