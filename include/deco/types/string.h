#ifndef deco_string_h
#define deco_string_h

#include "../InputStream.h"
#include "../OutputStream.h"
#include "../escape.h"
#include <string>

namespace deco
{
	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
	write(Stream& stream, const std::string& value)
	{
		stream.entry(escape_content(value));
	}

	template<typename I> constexpr
		void read(InputStream<I>& stream, std::string& value)
	{
		value = stream.parse_entry().content;	// deco parsing will already unescape
	}

	constexpr
	std::string& to_string(std::string& value)
	{
		return value;
	}

	constexpr
	const std::string& to_string(const std::string& value)
	{
		return value;
	}

	constexpr
	std::string&& to_string(std::string&& value)
	{
		return std::move(value);
	}
}

#endif//guard
