#ifndef deco_unescaped_string_h
#define deco_unescaped_string_h

#include "OutputStream.h"
#include <string>
#include <strong_type.h>


namespace deco
{
	STRONG_TYPE(unescaped_string, std::string);

	template<typename Stream>
	typename std::enable_if_t<std::is_base_of_v<deco::OutputStream, Stream>>
	write(Stream& stream, const deco::unescaped_string& value) {
		stream.entry(value);
	}

	void read(const deco::Entry& entry, deco::unescaped_string& value)
	{
		value = std::string(entry.content);
	}
}

#endif//guard