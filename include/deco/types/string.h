#ifndef deco_string_h
#define deco_string_h

#include "../OutputStream.h"
#include <string>

namespace deco
{
	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
	write(Stream& stream, const std::string& value)
	{
		stream.entry(escape_content(value));
	}

	void read(const std::string_view content, std::string& value)
	{
		value = unescape_content(content);
	}

	template<typename I> constexpr
		void read(InputStream<I>& stream, std::string& value)
	{
		read(stream.parse_entry().content, value);
	}
}

#endif//guard
