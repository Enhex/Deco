#ifndef deco_integral_h
#define deco_integral_h

#include "../InputStream.h"
#include "../OutputStream.h"

namespace deco
{
	template<typename T>
	struct is_single_entry<T, std::enable_if_t<std::is_integral_v<T>> > : std::true_type {};


	template<typename T> constexpr
	std::enable_if_t<std::is_integral_v<T>
	, std::string>
	to_string(const T& value)
	{
		return std::to_string(value);
	}


	//automatically provide default serialization implementation for arithmetic types
	template<typename Stream, typename T> constexpr
	std::enable_if_t<
		std::is_base_of_v<OutputStream, std::decay_t<Stream>> &&
		std::is_integral_v<T>
	>
	write(Stream& stream, const T& value) {
		stream.entry(to_string(value));
	}


	template<typename T> constexpr
	std::enable_if_t<std::is_integral_v<T>>
		read_content(const Content content, T& value)
	{
		using namespace boost::spirit::x3;
		phrase_parse(content.begin(), content.end(), get_integral_parser<T>(), ascii::space, value);

		//value = stoi(std::string(entry.content)); // no string_view/iterators support
	}

	template<typename I, typename T> constexpr
		std::enable_if_t<std::is_integral_v<T>>
		read(InputStream<I>& stream, T& value)
	{
		read_content(stream.parse_entry().content, value);

	}
}

#endif//guard
