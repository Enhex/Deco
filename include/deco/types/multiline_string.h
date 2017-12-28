#ifndef deco_multiline_string_h
#define deco_multiline_string_h

#include "../InputStream.h"
#include "../OutputStream.h"
#include <deco/types/string.h>
#include <string_view>
#include <strong_type.h>

namespace deco
{
	STRONG_TYPE(multiline_string, std::string);

	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
	write(Stream& stream, const multiline_string& value)
	{
		// in case of an empty string
		if (value.empty()) {
			stream.entry(value);
			return;
		}

		auto line_start = value.cbegin();
		auto iter = line_start;

		auto write_line = [&] {
			stream.entry(
				escape_content(std::string_view(&*line_start, std::distance(line_start, iter))));
		};

		while (iter != value.cend()) {
			if (*iter == entry_delimiter) {
				write_line();
				line_start += std::distance(line_start, ++iter);	// also skipping the entry delimiter
			}
			else // to avoid double advancing after skipping entry delimiter
				++iter;
		}

		write_line();
	}

	template<typename I>
	void read(InputStream<I>& stream, multiline_string& value)
	{
		value.clear();

		//NOTE: set-entry content should've been read already, now reading children
		if (!stream.peek_set_end()) {
			serialize(stream, static_cast<std::string&>(value));
		}

		std::string str;

		while (!stream.peek_set_end()) {
			serialize(stream, str);
			(value += '\n') += str;
		}
	}
}

#endif//guard
