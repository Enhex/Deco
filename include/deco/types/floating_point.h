#ifndef deco_floating_point_h
#define deco_floating_point_h

#include "../InputStream.h"
#include "../OutputStream.h"

#include <fmt/format.h>
#include <fast_float/fast_float.h>

namespace deco
{
	template<typename T>
	struct is_single_entry<T, std::enable_if_t<std::is_floating_point_v<T>> > : std::true_type {};

	template<typename T>
	std::enable_if_t<std::is_floating_point_v<T>
	, std::string>
	to_string(const T& value)
	{
		return fmt::to_string(value);
	}

	template<typename Stream, typename T> constexpr
	std::enable_if_t<
		std::is_base_of_v<OutputStream, std::decay_t<Stream>> &&
		std::is_floating_point_v<T>
	>
		write(Stream& stream, const T& value)
	{
		stream.entry(to_string(value));
	}

	template<typename I, typename T> constexpr
	std::enable_if_t<std::is_floating_point_v<T>>
		read(InputStream<I>& stream, T& value)
	{
		const auto content = stream.parse_entry().content;
		fast_float::from_chars(content.data(), content.data() + content.size(), value);
	}
}

#endif//guard
