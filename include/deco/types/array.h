#ifndef deco_std_array_h
#define deco_std_array_h

#include "../InputStream.h"
#include "../OutputStream.h"
#include <array>

namespace deco
{
	template<typename Stream, typename T, std::size_t N> constexpr
		std::enable_if_t< std::is_base_of_v<OutputStream, std::decay_t<Stream>> >
		write(Stream& stream, const std::array<T, N>& value)
	{
		write_elements(stream, value);
	}


	template<typename I, typename T, std::size_t N> constexpr
	void read(InputStream<I>& stream, std::array<T, N>& value)
	{
		typename std::array<T, N>::size_type index = 0;

		while (!stream.peek_set_end())
			serialize(stream, value[index++]);
	}
}

#endif//guard
