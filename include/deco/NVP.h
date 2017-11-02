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
		std::string name;
		T& value;
	};

	template<typename T>
	auto make_CNVP(std::string&& name, T& value) {
		return CNVP<T>{
			std::forward<std::string>(name),
				value	// want to always use T member, so no forwarding
		};
	}
}


namespace gs
{
	// NVP
	template<typename T>
	void write(deco::OutputStream& stream, const deco::NVP<T>& nvp)
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
			stream.parse_entry();		// skip set end if child didn't
		//TODO vector<T> already reads the set end, other types may not
	}


	// CNVP
	void write(deco::OutputStream& stream, const deco::CNVP<std::string>& nvp)
	{
		auto str = nvp.name + ": ";

		// escape content starting with whitespace or content delimiter
		const auto first = nvp.value.front();
		if (first == ' ' ||
			first == '\t' ||
			first == deco::content_delimiter)
			str += deco::content_delimiter;
		
		str += nvp.value;

		// escape content ending with structure delimiter
		const auto last = nvp.value.back();
		if (last == deco::structure_delimiter)
			str += deco::content_delimiter;

		serialize(stream, str);
	}

	template<typename T>
	void write(deco::OutputStream& stream, const deco::CNVP<T>& nvp)
	{
		if constexpr(std::is_floating_point_v<T>) {
			serialize(stream, (nvp.name + ": ") += deco::trim_float(std::to_string(nvp.value)));
		}
		else
			serialize(stream, (nvp.name + ": ") += std::to_string(nvp.value));
	}


	template<typename T>
	void read(deco::InputStream& stream, deco::CNVP<T>& nvp)
	{	
		auto entry = stream.parse_entry();

		// parse value
		auto value_start = entry.content.begin() + entry.content.find(':') + 1;
		// skip whitespace
		deco::skip_whitespace(value_start);
		// allow escaping whitespace
		if (*value_start == deco::content_delimiter)
			++value_start;

		// erase everything until the value start
		entry.content.remove_prefix(std::distance(entry.content.begin(), value_start));
		serialize(entry, nvp.value);
	}
}

#endif//guard