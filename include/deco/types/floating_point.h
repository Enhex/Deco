#ifndef deco_floating_point_h
#define deco_floating_point_h

#include "../InputStream.h"
#include "../OutputStream.h"

namespace deco
{
	template<typename T>
	struct is_single_entry<T, std::enable_if_t<std::is_floating_point_v<T>> > : std::true_type {};

	template<typename T>
	std::enable_if_t<std::is_floating_point_v<T>
	, std::string>
	to_string(const T& value)
	{
		std::string s;
		std::back_insert_iterator<std::string> sink(s);
		boost::spirit::karma::generate(sink, float_fixed<T>(), value);
		return s;
	}

	template<typename Stream, typename T> constexpr
	std::enable_if_t<
		std::is_base_of_v<OutputStream, std::decay_t<Stream>> &&
		std::is_floating_point_v<T>
	>
		write(Stream& stream, const T& value)
	{
		stream.entry(to_string(value));
		//stream.entry(trim_float(std::to_string(value)));
	}
	
	template<typename I, typename T> constexpr
	std::enable_if_t<std::is_floating_point_v<T>>
		read(InputStream<I>& stream, T& value)
	{
		const auto content = stream.parse_entry().content;

		using namespace boost::spirit::x3;
		phrase_parse(content.begin(), content.end(), real_parser<T>(), ascii::space, value);

		//value = stof(std::string(entry.content)); // no string_view/iterators support
	}
}

#endif//guard
