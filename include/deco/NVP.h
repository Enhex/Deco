#ifndef deco_NVP_h
#define deco_NVP_h

#include "InputStream.h"
#include "OutputStream.h"
#include <deco/types/arithmetic.h>
#include <deco/types/string.h>

namespace deco
{
	template<typename Stream, template<typename> typename NVP, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write_NVP(Stream& stream, const NVP<T>& nvp)
	{
		const auto str = [&nvp]() {
			return std::string(nvp.name) += ": ";
		};

		if constexpr (std::is_same_v<std::decay_t<T>, std::string>)
			serialize(stream, str() += escape_content(nvp.value));
		else if constexpr (std::is_floating_point_v<T>)
			serialize(stream, str() += to_string(nvp.value));
			//serialize(stream, str() += trim_float(std::to_string(nvp.value)));
		else
			serialize(stream, str() += std::to_string(nvp.value));
	}


	// Content name value pair: NVP inside the entry content, using the structure delimiter to separate between name and value, which means it can't be inside the name
	template<typename T>
	struct NVP_t {
		std::string& name;
		T& value;
	};

	template<typename T>
	constexpr auto make_NVP(std::string& name, T& value)
	{
		return NVP_t<T>{
			name,
			value
		};
	}

	template<typename Stream, typename T> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
	write(Stream& stream, const NVP_t<T>& nvp)
	{
		write_NVP(stream, nvp);
	}


	template<typename T, typename I> constexpr
	void read(InputStream<I>& stream, NVP_t<T>& nvp)
	{	
		auto content = stream.parse_entry().content;

		// skip whitespace
		const auto whitespace_end = skip_whitespace(content.begin());
		content.remove_prefix(std::distance(content.begin(), whitespace_end));

		// read name
		const auto name_end = content.begin() + content.find(':');
		const auto name_length = std::distance(content.begin(), name_end);
		nvp.name = content.substr(0, name_length);
		content.remove_prefix(name_length + 1);	// +1 for ':'

		// unescape delimiters chars
		unescape_content<true>(content);

		// parse value
		serialize(content, nvp.value);
	}
	template<typename T, typename I> constexpr
		void read(InputStream<I>& stream, NVP_t<T>&& nvp)
	{
		read(stream, nvp);
	}


	// version that doesn't read a name
	template<typename T>
	struct NVP_ignore_name_t {
		const std::string_view name;
		T& value;
	};

	template<typename T>
	constexpr auto make_NVP(std::string_view&& name, T& value)
	{
		return NVP_ignore_name_t<T>{
			std::forward<std::string_view>(name),
			value	// want to always use T member, so no forwarding
		};
	}

	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const NVP_ignore_name_t<T>& nvp)
	{
		write_NVP(stream, nvp);
	}

	template<typename T, typename I> constexpr
		void read(InputStream<I>& stream, NVP_ignore_name_t<T>& nvp)
	{
		auto content = stream.parse_entry().content;

		// skip whitespace
		const auto whitespace_end = skip_whitespace(content.begin() + content.find(':') + 1);
		content.remove_prefix(std::distance(content.begin(), whitespace_end));

		// unescape delimiters chars
		unescape_content<true>(content);

		// parse value
		serialize(content, nvp.value);
	}
	template<typename T, typename I> constexpr
		void read(InputStream<I>& stream, NVP_ignore_name_t<T>&& nvp)
	{
		read(stream, nvp);
	}
}

#endif//guard
