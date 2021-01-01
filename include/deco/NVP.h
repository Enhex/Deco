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
		const auto name_str = [&nvp]() {
			return std::string(nvp.name) += ": ";
		};

		/*NOTE:
		The name's start and value's end are automatically escaped by the std::string serialization.
		The value's internal sub-string start isn't escaped when serialized, so it needs to be escaped manually.
		The name's end doesn't need escaping.
		*/
		if constexpr (std::is_floating_point_v<T> || std::is_integral_v<T>)	// no need to escape
			serialize(stream, name_str() += to_string(nvp.value));
		else
			serialize(stream, name_str() += escape_content<true, false>(to_string(nvp.value)));
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
		// skip whitespace
		skip_whitespace(stream.position);

		// skip content begin delimiter
		if (*stream.position == content_delimiter)
			++stream.position;

		// read name
		const auto name_begin = stream.position;

		while (*stream.position != ':')
			++stream.position;

		nvp.name = std::string(name_begin, stream.position);

		// skip NVP delimiter
		++stream.position;	// +1 for ':'

		// read value
		serialize(stream, nvp.value);
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
		// skip name
		while (*(stream.position++) != ':');

		// read value
		serialize(stream, nvp.value);
	}
	template<typename T, typename I> constexpr
		void read(InputStream<I>& stream, NVP_ignore_name_t<T>&& nvp)
	{
		read(stream, nvp);
	}
}

#endif//guard
