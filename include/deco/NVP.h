#ifndef deco_NVP_h
#define deco_NVP_h

#include "InputStream.h"
#include "OutputStream.h"
#include <deco/types/string.h>
#include <gs/Serializer.h>

namespace deco
{
	// Content name value pair: NVP inside the entry content, using the structure delimiter to separate between name and value, which means it can't be inside the name
	template<typename T>
	struct NVP {
		std::string_view name;
		T& value;
	};

	template<typename T>
	constexpr auto make_NVP(std::string_view&& name, T& value)
	{
		return NVP<T>{
			std::forward<std::string_view>(name),
			value	// want to always use T member, so no forwarding
		};
	}

	// NVP
	template<typename Stream, typename T> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
	write(Stream& stream, const NVP<T>& nvp)
	{
		const auto str = [&nvp]() {
			return std::string(nvp.name) += ": ";
		};

		if constexpr (std::is_same_v<std::decay_t<T>, std::string>)
			gs::serialize(stream, str() += escape_content(nvp.value));
		else if constexpr (std::is_floating_point_v<T>)
			gs::serialize(stream, str() += to_string(nvp.value));
			//gs::serialize(stream, str() += trim_float(std::to_string(nvp.value)));
		else
			gs::serialize(stream, str() += std::to_string(nvp.value));
	}


	template<typename T, typename I> constexpr
	void read(InputStream<I>& stream, NVP<T>& nvp)
	{	
		auto entry = stream.parse_entry();

		// skip whitespace
		const auto whitespace_end = skip_whitespace(entry.content.begin() + entry.content.find(':') + 1);
		entry.content.remove_prefix(std::distance(entry.content.begin(), whitespace_end));

		// unescape delimiters chars
		unescape_content<true>(entry.content);

		// parse value
		read(entry, nvp.value);
	}
}

#endif//guard
