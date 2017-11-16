#ifndef deco_std_unordered_set_h
#define deco_std_unordered_set_h

#include "../entry_container.h"
#include <unordered_set>

namespace deco
{
	template<typename... Args>
	struct is_entry_container<std::unordered_set<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
		void read_elements(Stream& stream, std::unordered_set<Args...>& value)
	{
		typename std::unordered_set<Args...>::value_type input;

		while (!stream.peek_set_end()) {
			serialize(stream, input);
			value.emplace(input);
		}
	}
}

#endif//guard
