#ifndef deco_std_set_h
#define deco_std_set_h

#include "../entry_container.h"
#include <set>

namespace deco
{
	template<typename... Args>
	struct is_entry_container<std::set<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
		void read_elements(Stream& stream, std::set<Args...>& value)
	{
		typename std::set<Args...>::value_type input;

		while (!stream.peek_list_end()) {
			serialize(stream, input);
			value.emplace(input);
		}
	}
}

#endif//guard
