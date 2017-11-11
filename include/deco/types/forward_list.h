#ifndef deco_std_forward_list_h
#define deco_std_forward_list_h

#include "../entry_container.h"
#include <forward_list>

namespace deco
{
	template<typename... Args>
	struct is_entry_container<std::forward_list<Args...>> : std::true_type {};

	template<typename Stream, typename... Args> constexpr
	void read_elements(Stream&& stream, std::forward_list<Args...>& value)
	{
		//// find iterator to last element
		auto before_end = value.before_begin();
		for (auto& e : value)
			++before_end;

		while (!stream.peek_set_end()) {
			before_end = value.emplace_after(before_end);
			gs::serialize(stream, *before_end);
		}
	}
}

#endif//guard
