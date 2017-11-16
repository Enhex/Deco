#ifndef deco_set_container_h
#define deco_set_container_h

#include "InputStream.h"
#include "OutputStream.h"

namespace deco
{
	/* set container:
	each element is serialized as a set entry
	*/

	// set container trait
	template<typename T>
	struct is_set_container : std::false_type {};

	template<typename T>
	constexpr bool is_set_container_v = is_set_container<std::decay_t<T>>::value;

	// template for serializing entry containers
	template<typename Stream, typename T> constexpr
	std::enable_if_t<
		std::is_base_of_v<OutputStream, std::decay_t<Stream>> &&
		is_set_container_v<T>
	>
		write(Stream& stream, T& value)
	{
		write_element_sets(stream, value);
	}


	template<typename I, typename T> constexpr
	std::enable_if_t<is_set_container_v<T>>
	read(InputStream<I>& stream, T& value)
	{
		read_element_sets(stream, value);
	}


	// write the elements of a map type as sets.
	// If the key type can be serialized as a single entry, write it as a set-entry with the value serialized into it
	template<typename Stream, typename Map> constexpr
	void write_key_value_sets(Stream& stream, const Map& value)
	{
		for (auto& e : value)
		{
			if constexpr(is_single_entry_v<Map::key_type>)
			{
				stream.begin_set(to_string(e.first));
				serialize(stream, e.second);
				stream.end_set();
			}
			else
			{
				// key may require more than a single entry to be serialized
				stream.begin_set("key");
				serialize(stream, e.first);
				stream.end_set();
				stream.begin_set("value");
				serialize(stream, e.second);
				stream.end_set();
			}
		}
	}

	template<typename Stream, typename Map> constexpr
	void read_key_value_sets(Stream& stream, Map& value)
	{
		typename Map::key_type key_input;
		typename Map::mapped_type mapped_input;

		while (!stream.peek_set_end()) {
			if constexpr(is_single_entry_v<Map::key_type>)
			{
				serialize(stream, key_input);	// read set name
				serialize(stream, mapped_input);// read value
				stream.parse_entry();				// skip set end
			}
			else
			{
				serialize(stream, skip);		// skip set name
				serialize(stream, key_input);	// read value
				stream.parse_entry();				// skip set end

				serialize(stream, skip);		// skip set name
				serialize(stream, mapped_input);// read value
				stream.parse_entry();				// skip set end
			}

			value.emplace(std::make_pair(key_input, mapped_input));
		}
	}
}

#endif//guard
