#ifndef deco_string_h
#define deco_string_h

#include "OutputStream.h"
#include <string>

namespace deco
{
	template<typename Stream>
	typename std::enable_if_t<std::is_base_of_v<deco::OutputStream, Stream>>
	write(Stream& stream, const std::string& value)
	{
		stream.entry(deco::escape_content(value));
	}

	void read(const deco::Entry& entry, std::string& value)
	{
		value = deco::unescape_content(entry.content);
	}
}

#endif//guard
