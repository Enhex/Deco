#ifndef deco_std_list_h
#define deco_std_list_h

#include "../entry_container.h"
#include <list>

namespace deco
{
	template<typename... Args>
	struct is_entry_container<std::list<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
	void read_elements(Stream&& stream, std::list<Args...>& value)
	{
		while (!stream.peek_set_end())
			serialize(stream, value.emplace_back());
	}
}

#endif//guard
