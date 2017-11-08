#ifndef deco_integral_h
#define deco_integral_h

#include "../OutputStream.h"
#include <gs/Serializer.h>

namespace deco
{
	//automatically provide default serialization implementation for arithmetic types
	template<typename Stream, typename T> constexpr
	std::enable_if_t<
		std::is_base_of_v<OutputStream, std::decay_t<Stream>> &&
		std::is_integral_v<T>
	>
	write(gs::Serializer<Stream>& serializer, const T& value) {
		serializer.stream.entry(std::to_string(value));
	}

	template<typename T> constexpr
	std::enable_if_t<std::is_integral_v<T>>
		read(const Entry& entry, T& value)
	{
		using namespace boost::spirit::x3;
		phrase_parse(entry.content.begin(), entry.content.end(), get_integral_parser<T>(), ascii::space, value);

		//value = stoi(std::string(entry.content)); // no string_view/iterators support
	}
}

#endif//guard
