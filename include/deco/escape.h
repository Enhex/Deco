#ifndef deco_escape_h
#define deco_escape_h

namespace deco
{
	// escape content delimiters
	template<bool escape_begin = true, bool escape_end = true>
	auto escape_content(const std::string_view content)
	{
		std::string str;

		if (content.empty())
			return str;

		if constexpr(escape_begin)
		{
			// escape content starting with whitespace or content delimiter
			const auto first = content.front();
			if (first == ' ' ||
				first == '\t' ||
				first == deco::content_delimiter)
				str.insert(str.begin(), deco::content_delimiter);
		}

		str += content;


		if constexpr(escape_end)
		{
			// escape content ending with content delimiter or structure delimiter
			const auto last = content.back();
			if (last == deco::structure_delimiter ||
				last == deco::content_delimiter)
				str += deco::content_delimiter;
		}

		return str;
	}

	// remove delimiters used to escape content
	template<bool unescape_content_begin = false> constexpr	// entry parsing already removes content begin delimiter
	void unescape_content(std::string_view& content)
	{
		// erase start content delimiter
		if constexpr (unescape_content_begin) {
			if (content.front() == content_delimiter)
				content.remove_prefix(1);
		}

		// erase end content delimiter
		if (content.back() == content_delimiter)
			content.remove_suffix(1);
	}

	// take const& and return a copy
	template<bool unescape_content_begin = false> constexpr
	auto unescape_content(const std::string_view& ref_content)
	{
		auto content = ref_content;
		unescape_content<unescape_content_begin>(content);
		return content;
	}

	// rvalue
	template<bool unescape_content_begin = false> constexpr
	auto unescape_content(std::string_view&& content)
	{
		unescape_content<unescape_content_begin>(content);
		return content;
	}

	// string
	template<bool unescape_content_begin = false> constexpr
	void unescape_content(std::string& content)
	{
		// erase start content delimiter
		if constexpr (unescape_content_begin) {
			if (content.front() == content_delimiter)
				content.erase(0, 1);
		}

		// erase end content delimiter
		if (content.back() == content_delimiter)
			content.pop_back();
	}
}

#endif//guard