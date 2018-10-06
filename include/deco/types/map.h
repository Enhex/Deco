#ifndef deco_std_map_h
#define deco_std_map_h

#include "../list_container.h"
#include <map>

namespace deco
{
	template<typename... Args>
	struct is_list_container<std::map<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
		void write_element_lists(Stream& stream, std::map<Args...>& value)
	{
		write_element_lists(stream, value);
	}

	template<typename Stream, typename... Args> constexpr
	void read_element_lists(Stream& stream, std::map<Args...>& value)
	{
		read_key_value_lists(stream, value);
	}
}

#endif//guard
