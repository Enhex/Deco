#ifndef deco_list_container_h
#define deco_list_container_h

#include "InputStream.h"
#include "OutputStream.h"

namespace deco
{
	/* list container:
	each element is serialized as a list entry
	*/

	// list container trait
	template<typename T>
	struct is_list_container : std::false_type {};

	template<typename T>
	constexpr bool is_list_container_v = is_list_container<std::decay_t<T>>::value;

	// template for serializing entry containers
	template<typename Stream, typename T> constexpr
	std::enable_if_t<
		std::is_base_of_v<OutputStream, std::decay_t<Stream>> &&
		is_list_container_v<T>
	>
		write(Stream& stream, T& value)
	{
		write_element_lists(stream, value);
	}


	template<typename I, typename T> constexpr
	std::enable_if_t<is_list_container_v<T>>
	read(InputStream<I>& stream, T& value)
	{
		read_element_lists(stream, value);
	}


	// write the elements of a map type as lists.
	// If the key type can be serialized as a single entry, write it as a list-entry with the value serialized into it
	template<typename Stream, typename Map> constexpr
	void write_key_value_lists(Stream& stream, const Map& value)
	{
		for (auto& e : value)
		{
			if constexpr(is_single_entry_v<typename Map::key_type>)
			{
				if constexpr(std::is_base_of_v<std::string, std::decay_t<decltype(e.first)>>)
					stream.begin_list(e.first);
				else
					stream.begin_list(to_string(e.first));
				serialize(stream, e.second);
				stream.end_list();
			}
			else
			{
				// key may require more than a single entry to be serialized
				stream.begin_list("key");
				serialize(stream, e.first);
				stream.end_list();
				stream.begin_list("value");
				serialize(stream, e.second);
				stream.end_list();
			}
		}
	}

	template<typename Stream, typename Map> constexpr
	void read_key_value_lists(Stream& stream, Map& value)
	{
		typename Map::key_type key_input;
		typename Map::mapped_type mapped_input;

		while (!stream.peek_list_end()) {
			if constexpr(is_single_entry_v<typename Map::key_type>)
			{
				serialize(stream, key_input);	// read list name
				serialize(stream, mapped_input);// read value
				stream.parse_entry();			// skip list end
			}
			else
			{
				serialize(stream, skip);		// skip list name
				serialize(stream, key_input);	// read value
				stream.parse_entry();			// skip list end

				serialize(stream, skip);		// skip list name
				serialize(stream, mapped_input);// read value
				stream.parse_entry();			// skip list end
			}

			value.emplace(std::make_pair(key_input, mapped_input));
		}
	}
}

#endif//guard
