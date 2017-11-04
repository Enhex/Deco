#ifndef deco_multiline_string_h
#define deco_multiline_string_h

#include "InputStream.h"
#include "OutputStream.h"
#include <string>
#include <string_view>
#include <strong_type.h>


namespace deco
{
	STRONG_TYPE(multiline_string, std::string);
}

namespace gs
{
	template<typename Stream>
	typename std::enable_if_t<std::is_base_of_v<deco::OutputStream, Stream>>
	write(Stream& stream, const deco::multiline_string& value)
	{
		auto line_start = value.cbegin();
		auto iter = line_start;

		auto write_line = [&] {
			stream.entry(
				deco::escape_content(std::string_view(&*line_start, std::distance(line_start, iter))));
		};

		for (; iter != value.cend(); ++iter) {
			if (*iter == deco::entry_delimiter) {
				write_line();
				line_start += std::distance(line_start, ++iter);
			}
		}

		write_line();
	}

	void read(deco::InputStream& stream, deco::multiline_string& value)
	{
		deco::escaped_string str;

		//NOTE: set-entry content should've been read already, now reading children
		if (!stream.peek_set_end()) {
			serialize(stream, str);
			value += str;
		}
		while (!stream.peek_set_end()) {
			serialize(stream, str);
			(value += '\n') += str;
		}
		stream.parse_entry();	// skip set end
	}
}

#endif//guard