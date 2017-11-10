#ifndef deco_multiline_string_h
#define deco_multiline_string_h

#include "../InputStream.h"
#include "../OutputStream.h"
#include <deco/types/string.h>
#include <gs/serializer.h>
#include <string_view>
#include <strong_type.h>

namespace deco
{
	STRONG_TYPE(multiline_string, std::string);

	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
	write(Stream& stream, const multiline_string& value)
	{
		auto line_start = value.cbegin();
		auto iter = line_start;

		auto write_line = [&] {
			stream.entry(
				escape_content(std::string_view(&*line_start, std::distance(line_start, iter))));
		};

		for (; iter != value.cend(); ++iter) {
			if (*iter == entry_delimiter) {
				write_line();
				line_start += std::distance(line_start, ++iter);
			}
		}

		write_line();
	}

	template<typename I>
	void read(InputStream<I>& stream, multiline_string& value)
	{
		std::string str;

		//NOTE: set-entry content should've been read already, now reading children
		if (!stream.peek_set_end()) {
			gs::serializer(stream, str);
			value += str;
		}
		while (!stream.peek_set_end()) {
			gs::serializer(stream, str);
			(value += '\n') += str;
		}
	}
}

#endif//guard
