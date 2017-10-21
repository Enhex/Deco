#ifndef deco_InputStream_h
#define deco_InputStream_h

#include "deco.h"
#include <Generic Serialization/Header.h>
#include <string>
#include <vector>

namespace deco
{
	// Used to construct deco document string on the fly
	/*TODO
	- use variadic template to read N root entries as types?
	What if the number of root entries is dynamic?
		- allow reading in a loop
	- try reading into an Entry tree for a start/test.
	*/
	struct InputStream
	{
		InputStream(std::string&& new_str) :
			str(new_str),
			position(str.begin())
		{}

		std::string str;
		std::string::iterator position;


		Entry parse_entry() {
			return deco::parse_entry(position, str.end());
		}
	};
}

namespace gs
{
	template<>
	struct is_input<deco::InputStream> : std::true_type {};


	template<typename T>
	void read(deco::InputStream& stream, T& value)
	{
		read(stream.parse_entry(), value);
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

	template<typename T>
	void read(deco::InputStream& stream, std::vector<T>& value)
	{
		//NOTE: set entry content should've been read already, now reading children
		for (auto entry = stream.parse_entry(); entry.type != deco::Entry::set_end; entry = stream.parse_entry())
			read(entry, value.emplace_back());
	}
}

#endif//guard