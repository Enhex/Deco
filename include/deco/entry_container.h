#ifndef deco_entry_container_h
#define deco_entry_container_h

#include "InputStream.h"
#include "OutputStream.h"

namespace deco
{
	/* entry container:
	each element is serialized as a single entry
	*/

	// entry container trait
	template<typename T>
	struct is_entry_container : std::false_type {};

	template<typename T>
	constexpr bool is_entry_container_v = is_entry_container<std::decay_t<T>>::value;

	// template for serializing entry containers
	template<typename Stream, typename T> constexpr
	std::enable_if_t<
		std::is_base_of_v<OutputStream, std::decay_t<Stream>> &&
		is_entry_container_v<T>
	>
		write(Stream& stream, T& value)	//NOTE: calling serialize prevents using const since it's used for both read & write
	{
		write_elements(stream, value);
	}


	template<typename I, typename T> constexpr
	std::enable_if_t<is_entry_container_v<T>>
	read(InputStream<I>& stream, T& value)
	{
		read_elements(stream, value);
	}
}

#endif//guard
