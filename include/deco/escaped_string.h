#ifndef deco_escaped_string_h
#define deco_escaped_string_h

#include "InputStream.h"
#include "OutputStream.h"
#include <string>
#include <strong_type.h>


namespace deco
{
	STRONG_TYPE(escaped_string, std::string);
}

namespace gs
{
	template<typename Stream>
	typename std::enable_if_t<std::is_base_of_v<deco::OutputStream, Stream>>
	write(Stream& stream, const deco::escaped_string& value)
	{
		stream.entry(deco::escape_content(value));
	}

	void read(const deco::Entry& entry, deco::escaped_string& value)
	{
		value = std::string(entry.content);
		deco::unescape_content(value);
	}
}

#endif//guard