#ifndef deco_std_vector_h
#define deco_std_vector_h

#include "../entry_container.h"
#include <vector>

namespace deco
{
	template<typename... Args>
	struct is_entry_container<std::vector<Args...>> : std::true_type {};
}

#endif//guard
