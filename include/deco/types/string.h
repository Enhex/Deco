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

	void read(const Entry& entry, std::string& value)
	{
		value = unescape_content(entry.content);
	}
}

#endif//guard
