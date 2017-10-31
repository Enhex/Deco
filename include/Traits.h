#ifndef deco_Traits_h
#define deco_Traits_h

namespace gs
{
	// deco trait
	template<typename T>
	struct is_deco : std::false_type {};

	template<typename T>
	constexpr bool is_deco_v = is_deco<T>::value;
}

#endif//guard