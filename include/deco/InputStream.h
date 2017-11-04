#ifndef deco_InputStream_h
#define deco_InputStream_h

#include "Traits.h"
#include "deco.h"
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/numeric.hpp>
#include <gs/Core.h>
#include <string>
#include <vector>

namespace deco
{
	struct InputStream
	{
		InputStream(std::string::const_iterator begin) : position(begin) {}

		std::string::const_iterator position;

		Entry current_entry;

		const Entry& parse_entry() {
			return current_entry = deco::parse_entry(position);
		}

		Entry peek_entry() const {
			return deco::peek_entry(position);
		}

		bool peek_set_end() const {
			return deco::peek_set_end(position);
		}
	};

	// used to skip an entry without parsing it into a type
	struct skip_t {};
	constexpr skip_t skip;
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

	// skip entry without parsing
	template<typename Stream>
	typename std::enable_if_t<
		is_deco_v<Stream> &&
		is_input_v<Stream>
	>
	serialize(Stream& stream, const deco::skip_t&)
	{
		stream.parse_entry();
	}

	template<>
	void serialize(deco::Entry& entry, const deco::skip_t&)
	{
	}


	template<typename T>
	void read(deco::InputStream& stream, std::vector<T>& value)
	{
		//NOTE: set-entry content should've been read already, now reading children
		while (!stream.peek_set_end())
			serialize(stream, value.emplace_back());
		stream.parse_entry();	// skip set end
	}

	template<typename T>
	constexpr auto get_integral_parser() {
		if constexpr(std::is_signed_v<T>)
			return int_parser<T>();
		else
			return uint_parser<T>();
	}

	template<typename T>
	typename std::enable_if_t<std::is_integral_v<T>>
	read(const deco::Entry& entry, T& value)
	{
		using namespace boost::spirit::x3;
		phrase_parse(entry.content.begin(), entry.content.end(), get_integral_parser<T>(), ascii::space, value);

		//value = stoi(std::string(entry.content)); // no string_view/iterators support
	}

	template<typename T>
	typename std::enable_if_t<std::is_floating_point_v<T>>
	read(const deco::Entry& entry, T& value)
	{
		using namespace boost::spirit::x3;
		phrase_parse(entry.content.begin(), entry.content.end(), real_parser<T>(), ascii::space, value);

		//value = stof(std::string(entry.content)); // no string_view/iterators support
	}

	void read(const deco::Entry& entry, std::string& value)
	{
		value = entry.content;
	}
}

#endif//guard