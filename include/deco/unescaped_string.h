#ifndef deco_unescaped_string_h
#define deco_unescaped_string_h

#include "InputStream.h"
#include "OutputStream.h"
#include <gs/serializer.h>
#include <string>
#include <strong_type.h>

namespace deco
{
	STRONG_TYPE(unescaped_string, std::string);

	template<typename Stream>
	typename std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
	write(gs::Serializer<Stream>& serializer, unescaped_string& value) {
		serializer.stream.entry(value);
	}
	
	void read(const deco::Entry& entry, unescaped_string& value)
	{
		value = std::string(entry.content);
	}
	/*
	void read(gs::Serializer<InputStream&>& serializer, unescaped_string& value)
	{
		auto entry = serializer.stream.parse_entry();
		value = std::string(entry.content);
	}
	*/
	/* works
	template<typename Stream>
	void read(gs::Serializer<Stream>& serializer, unescaped_string& value)
	{
	auto entry = serializer.stream.parse_entry();
	value = std::string(entry.content);
	}*/
}

#endif//guard