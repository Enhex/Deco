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
	void write(deco::OutputStream& stream, const deco::escaped_string& value)
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