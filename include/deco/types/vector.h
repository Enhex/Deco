#ifndef deco_std_vector_h
#define deco_std_vector_h

#include "../entry_container.h"
#include <vector>

namespace deco
{
	template<typename... Args>
	struct is_entry_container<std::vector<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
	void read_elements(Stream&& stream, std::vector<Args...>& value)
	{
		//NOTE: set-entry content should've been read already, now reading children
		while (!stream.peek_set_end())
			serialize(stream, value.emplace_back());
		//NOTE: set end will be skipped by the caller
	}
}

#endif//guard
