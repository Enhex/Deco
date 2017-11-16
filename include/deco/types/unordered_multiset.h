#ifndef deco_std_unordered_multiset_h
#define deco_std_unordered_multiset_h

#include "../entry_container.h"
#include <unordered_set>

namespace deco
{
	template<typename... Args>
	struct is_entry_container<std::unordered_multiset<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
		void read_elements(Stream& stream, std::unordered_multiset<Args...>& value)
	{
		typename std::unordered_multiset<Args...>::value_type input;

		while (!stream.peek_set_end()) {
			serialize(stream, input);
			value.emplace(input);
		}
	}
}

#endif//guard
