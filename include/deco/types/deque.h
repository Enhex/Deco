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
		//NOTE: set-entry content should've been read already, now reading children
		while (!stream.peek_set_end())
			gs::serialize(stream, value.emplace_back());
		//NOTE: set end will be skipped by the caller
	}
}

#endif//guard
