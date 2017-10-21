#ifndef deco_Serialization_h
#define deco_Serialization_h

#include "deco.h"
#include <string>

#include <vector>
#include <Generic Serialization/Header.h>

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
		std::string str;
	};
}

namespace gs
{
	template<>
	struct is_input<deco::InputStream> : std::true_type {};


	// parse next entry as a type
	void read(const deco::InputStream& stream, char& value)
	{
		value = stream.content[0];
	}

	void read(const deco::InputStream& stream, int& value)
	{
		value = stoi(std::string(entry.content)); // no string_view/iterators support
	}

	void read(const deco::InputStream& stream, float& value)
	{
		value = stof(std::string(entry.content)); // no string_view/iterators support
	}

	void read(const deco::InputStream& stream, std::string& value)
	{
		value = entry.content;
	}

	template<typename T>
	void read(const deco::InputStream& stream, std::vector<T>& value)
	{
		T temp;

		for (const auto& child : entry.entries) {
			from_entry(temp, child);
			value.emplace_back(temp);
		}
	}
}

#endif//guard