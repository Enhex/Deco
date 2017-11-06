#ifndef deco_InputStream_h
#define deco_InputStream_h

#include <boost/spirit/home/x3.hpp>

// used to fix boost spirit karma signbit error bug
namespace boost::spirit::detail {
	using boost::spirit::x3::signbit;
}

#include <boost/spirit/home/x3/numeric.hpp>

#include "Traits.h"
#include "Deco.h"
#include <gs/Core.h>

#include "string.h"

namespace deco
{
	struct InputStream
	{
		InputStream(std::string::const_iterator begin) : position(begin) {}

		std::string::const_iterator position;

		Entry parse_entry() {
			return deco::parse_entry(position);
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


	template<typename T>
	constexpr auto get_integral_parser() {
		using namespace boost::spirit::x3;
		if constexpr(std::is_signed_v<T>)
			return int_parser<T>();
		else
			return uint_parser<T>();
	}


	/*TODO disables non-specialized?
	template<typename T>
	void read(T& value);*/

	template<typename T>
	void read(deco::InputStream& stream, T& value)
	{
		deco::read(stream.parse_entry(), value);
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
		deco::read(stream, value);
	}

	template<typename T>
	void serialize(deco::Entry& entry, T& value)
	{
		deco::read(entry, value);
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
}

#endif//guard
