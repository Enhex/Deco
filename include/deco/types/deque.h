#ifndef deco_std_deque_h
#define deco_std_deque_h

#include "../entry_container.h"
#include <deque>

namespace deco
{
	template<typename... Args>
	struct is_entry_container<std::deque<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
	void read_elements(Stream&& stream, std::deque<Args...>& value)
	{
		while (!stream.peek_list_end())
			serialize(stream, value.emplace_back());
	}
}

#endif//guard
