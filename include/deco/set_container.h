#ifndef deco_set_container_h
#define deco_set_container_h

#include "InputStream.h"
#include "OutputStream.h"

namespace deco
{
	/* set container:
	each element is serialized as a set entry
	*/

	// set container trait
	template<typename T>
	struct is_set_container : std::false_type {};

	template<typename T>
	constexpr bool is_set_container_v = is_set_container<std::decay_t<T>>::value;

	// template for serializing entry containers
	template<typename Stream, typename T> constexpr
	std::enable_if_t<
		std::is_base_of_v<OutputStream, std::decay_t<Stream>> &&
		is_set_container_v<T>
	>
		write(Stream& stream, T& value)
	{
		write_element_sets(stream, value);
	}


	template<typename I, template<typename...> typename T, typename... Args> constexpr
	std::enable_if_t< is_set_container_v<T<Args...>> >
	read(InputStream<I>& stream, T<Args...>& value)
	{
		read_element_sets(stream, value);
	}
}

#endif//guard
