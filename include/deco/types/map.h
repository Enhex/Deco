#ifndef deco_std_map_h
#define deco_std_map_h

#include "../set_container.h"
#include <map>

namespace deco
{
	template<typename... Args>
	struct is_set_container<std::map<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
	void write_element_sets(Stream& stream, const std::map<Args...>& value)
	{
		constexpr auto single_entry = is_single_entry_v<std::map<Args...>::key_type>;

		for (auto& e : value)
		{
			if constexpr(single_entry)
			{
				stream.begin_set(to_string(e.first));
					gs::serialize(stream, e.second);
				stream.end_set();
			}
			else
			{
				// key may require more than a single entry to be serialized
				stream.begin_set("key");
					gs::serialize(stream, e.first);
				stream.end_set();
				stream.begin_set("value");
					gs::serialize(stream, e.second);
				stream.end_set();
			}
		}
	}

	template<typename Stream, typename... Args> constexpr
	void read_element_sets(Stream& stream, std::map<Args...>& value)
	{
		constexpr auto single_entry = is_single_entry_v<std::map<Args...>::key_type>;

		typename std::map<Args...>::key_type key_input;
		typename std::map<Args...>::mapped_type mapped_input;

		while (!stream.peek_set_end()) {
			if constexpr(single_entry)
			{
				gs::serialize(stream, key_input);	// read set name
				gs::serialize(stream, mapped_input);// read value
				stream.parse_entry();				// skip set end
			}
			else
			{
				gs::serialize(stream, skip);		// skip set name
				gs::serialize(stream, key_input);	// read value
				stream.parse_entry();				// skip set end

				gs::serialize(stream, skip);		// skip set name
				gs::serialize(stream, mapped_input);// read value
				stream.parse_entry();				// skip set end
			}

			value.emplace(std::make_pair(key_input, mapped_input));
		}
	}
}

#endif//guard
