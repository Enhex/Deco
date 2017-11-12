#ifndef deco_Traits_h
#define deco_Traits_h

#include <type_traits>

namespace deco
{
	// trait for types that can be serialized as a single entry
	template<typename T, typename Enable = void>
	struct is_single_entry : std::false_type {};

	template<typename T>
	constexpr bool is_single_entry_v = is_single_entry<std::decay_t<T>>::value;
}

namespace gs
{
	// deco trait
	template<typename T>
	struct is_deco : std::false_type {};

	template<typename T>
	constexpr bool is_deco_v = is_deco<std::decay_t<T>>::value;
}

#endif//guard
