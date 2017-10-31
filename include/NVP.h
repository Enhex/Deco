#ifndef deco_NVP_h
#define deco_NVP_h

#include "InputStream.h"
#include "OutputStream.h"
#include <string>


namespace deco
{
	// name value pair
	template<typename T>
	struct NVP
	{
		std::string name;
		T& value;
	};

	template<typename T>
	auto make_NVP(std::string&& name, T& value) {
		return NVP<T>{
			std::forward<std::string>(name),
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
		serialize(stream, nvp.name);	// read set entry
		serialize(stream, nvp.value);	// read child entry
		if(stream.current_entry.type != deco::Entry::set_end)
			stream.parse_entry();		// skip set end if child didn't
		//TODO vector<T> already reads the set end, other types may not
	}


	template<typename T>
	void write(deco::OutputStream& stream, const deco::CNVP<T>& nvp)
	{
		std::stringstream ss;
		ss << nvp.name << ": " << nvp.value;
		serialize(stream, ss.str());
	}


	template<typename T>
	void read(deco::InputStream& stream, deco::CNVP<T>& nvp)
	{	
		auto entry = stream.parse_entry();
		const auto original_content = entry.content;
		const auto pos = original_content.find(':');

		// parse name
		entry.content = original_content.substr(0, pos);
		serialize(entry, nvp.name);

		// parse value
		entry.content = original_content.substr(pos+1, original_content.length() - pos);
		serialize(entry, nvp.value);
	}
}

#endif//guard