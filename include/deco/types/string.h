#ifndef deco_string_h
#define deco_string_h

#include "../OutputStream.h"
#include <string>
#include <gs/Serializer.h>

namespace deco
{
	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
	write(gs::Serializer<Stream>& serializer, const std::string& value)
	{
		serializer.stream.entry(escape_content(value));
	}

	void read(const Entry& entry, std::string& value)
	{
		value = unescape_content(entry.content);
	}
}

#endif//guard
