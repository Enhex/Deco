#ifndef deco_InputStream_h
#define deco_InputStream_h

#include "Traits.h"
#include "deco.h"
#include <Generic Serialization/Core.h>
#include <string>
#include <vector>

namespace deco
{
	struct InputStream
	{
		InputStream(std::string&& new_str) :
			str(new_str),
			position(str.begin())
		{}

		std::string str;
		std::string::iterator position;

		Entry current_entry;

		const Entry& parse_entry() {
			return current_entry = deco::parse_entry(position, str.end());
		}
	};
}

namespace gs
{
	template<>
	struct is_input<deco::InputStream> : std::true_type {};

	template<>
	struct is_deco<deco::InputStream> : std::true_type {};


	// serialize input deco
	template<typename Stream, typename T>
	typename std::enable_if_t<
		is_deco_v<Stream> &&
		is_input_v<Stream>
	>
	serialize(Stream& stream, T& value)
	{
		read(stream, value);
	}

	template<typename T>
	void read(deco::InputStream& stream, T& value)
	{
		read(stream.parse_entry(), value);
	}

	template<typename T>
	void serialize(deco::Entry& entry, T& value)
	{
		read(entry, value);
	}


	template<typename T>
	void read(deco::InputStream& stream, std::vector<T>& value)
	{
		//NOTE: set entry content should've been read already, now reading children
		for (auto entry = stream.parse_entry(); entry.type != deco::Entry::set_end; entry = stream.parse_entry())
			read(entry, value.emplace_back());
	}

	void read(const deco::Entry& entry, char& value)
	{
		value = entry.content[0];
	}

	void read(const deco::Entry& entry, int& value)
	{
		value = stoi(std::string(entry.content)); // no string_view/iterators support
	}

	void read(const deco::Entry& entry, float& value)
	{
		value = stof(std::string(entry.content)); // no string_view/iterators support
	}

	void read(const deco::Entry& entry, std::string& value)
	{
		value = entry.content;
	}
}

#endif//guard