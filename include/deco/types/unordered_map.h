#ifndef deco_std_unordered_map_h
#define deco_std_unordered_map_h

#include "../set_container.h"
#include <map>

namespace deco
{
	template<typename... Args>
	struct is_set_container<std::unordered_map<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
	void write_element_sets(Stream& stream, const std::unordered_map<Args...>& value)
	{
		write_key_value_sets(stream, value);
	}

	template<typename Stream, typename... Args> constexpr
	void read_element_sets(Stream& stream, std::unordered_map<Args...>& value)
	{
		read_key_value_sets(stream, value);
	}
}

#endif//guard
