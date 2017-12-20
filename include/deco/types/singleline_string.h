#ifndef deco_unescaped_string_h
#define deco_unescaped_string_h

#include "../InputStream.h"
#include "../OutputStream.h"
#include "string.h"
#include <strong_type.h>

namespace deco
{
	// strong type for serializing string as a single entry.
	// does not verify the content is actually a single line.
	STRONG_TYPE(singleline_string, std::string);

	template<>
	struct is_single_entry<singleline_string> : std::true_type {};

	template<typename Stream> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const singleline_string& value)
	{
		write(stream, static_cast<const std::string&>(value));
	}

	template<typename I> constexpr
		void read(InputStream<I>& stream, singleline_string& value)
	{
		read(stream, static_cast<std::string&>(value));
	}
}

#endif//guard