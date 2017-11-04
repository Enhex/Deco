#ifndef deco_NVP_h
#define deco_NVP_h

#include "InputStream.h"
#include "OutputStream.h"
#include <string>


namespace deco
{
	// name value pair for serialization
	template<typename T>
	struct NVP
	{
		std::string_view name;
		T& value;
	};

	//NOTE: class template argument deduction not supported yet
	template<typename T>
	auto make_NVP(std::string_view&& name, T& value) {
		return NVP<T>{
			std::forward<std::string_view>(name),
				value	// want to always use T member, so no forwarding
		};
	}


	// Content name value pair: NVP inside the entry content, using the structure delimiter to separate between name and value, which means it can't be inside the name
	template<typename T>
	struct CNVP {
		std::string_view name;
		T& value;
	};

	template<typename T>
	auto make_CNVP(std::string_view&& name, T& value) {
		return CNVP<T>{
			std::forward<std::string_view>(name),
				value	// want to always use T member, so no forwarding
		};
	}
}


namespace gs
{
	// NVP
	template<typename Stream, typename T>
	typename std::enable_if_t<std::is_base_of_v<deco::OutputStream, Stream>>
	write(Stream& stream, const deco::NVP<T>& nvp)
	{
		stream.begin_set(nvp.name);
		serialize(stream, nvp.value);
		stream.end_set();
	}

	template<typename T>
	void read(deco::InputStream& stream, deco::NVP<T>& nvp)
	{
		serialize(stream, deco::skip);	// skip set entry name
		serialize(stream, nvp.value);	// read child entry
		if(stream.current_entry.type != deco::Entry::set_end)
			stream.parse_entry();		// skip set end if T didn't
	}


	// CNVP
	template<typename Stream, typename T>
	typename std::enable_if_t<std::is_base_of_v<deco::OutputStream, Stream>>
	write(Stream& stream, const deco::CNVP<T>& nvp)
	{
		const auto str = [&nvp]() {
			return std::string(nvp.name) += ": ";
		};

		if constexpr (std::is_same_v<T, std::string>)
			serialize(stream, str() += deco::escape_content(nvp.value));
		else if constexpr (std::is_floating_point_v<T>)
			serialize(stream, str() += deco::to_string(nvp.value));
			//serialize(stream, str() += deco::trim_float(std::to_string(nvp.value)));
		else
			serialize(stream, str() += std::to_string(nvp.value));
	}


	template<typename T>
	void read(deco::InputStream& stream, deco::CNVP<T>& nvp)
	{	
		auto entry = stream.parse_entry();

		// skip whitespace
		auto whitespace_end = entry.content.begin() + entry.content.find(':') + 1;
		deco::skip_whitespace(whitespace_end);
		entry.content.remove_prefix(std::distance(entry.content.begin(), whitespace_end));

		// unescape delimiters chars
		deco::unescape_content<true>(entry.content);

		// parse value
		serialize(entry, nvp.value);
	}
}

#endif//guard