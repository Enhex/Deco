#ifndef deco_InputStream_h
#define deco_InputStream_h

#include <boost/spirit/home/x3.hpp>

// used to fix boost spirit karma signbit error bug
namespace boost::spirit::detail {
	using boost::spirit::x3::signbit;
}

#include <boost/spirit/home/x3/numeric.hpp>

#include "Deco.h"
#include "string.h"
#include "traits.h"
#include <gs/Serializer.h>
#include <gs/traits.h>

namespace deco
{
	//TODO templatize iterator type
	struct InputStream
	{
		InputStream(std::string::const_iterator begin) noexcept : position(begin) {}

		std::string::const_iterator position;

		constexpr Entry parse_entry() {
			return deco::parse_entry(position);
		}

		constexpr Entry peek_entry() const {
			return deco::peek_entry(position);
		}

		constexpr bool peek_set_end() const {
			return deco::peek_set_end(position);
		}
	};

	// used to skip an entry without parsing it into a type
	struct skip_t {};
	constexpr skip_t skip;


	template<typename T>
	constexpr auto get_integral_parser()
	{
		using namespace boost::spirit::x3;

		if constexpr(std::is_signed_v<T>)
			return int_parser<T>();
		else
			return uint_parser<T>();
	}

	template<typename T> constexpr
	void read(gs::Serializer<InputStream&>& serializer, T& value)
	{
		read(serializer.stream.parse_entry(), value);
	}
}

namespace gs
{
	template<> struct is_input<deco::InputStream> : std::true_type {};

	template<> struct is_deco<deco::InputStream> : std::true_type {};

	template<typename T>
	constexpr auto is_deco_input_v = is_deco_v<T> && is_input_v<T>;


	// serialize input deco
	template<typename Stream, typename T> constexpr
	std::enable_if_t<is_deco_input_v<Stream>>
	serialize(Serializer<Stream>& serializer, T& value)
	{
		deco::read(serializer, value);
	}

	/*template<typename T>
	void serialize(deco::Entry& entry, T& value)
	{
		deco::read(entry, value);
	}*/


	// skip entry without parsing
	template<typename Stream> constexpr
	std::enable_if_t<is_deco_input_v<Stream>>
	serialize(Serializer<Stream>& serializer, const deco::skip_t&)
	{
		serializer.stream.parse_entry();
	}

	constexpr void serialize(deco::Entry& entry, const deco::skip_t&)
	{
	}
}

#endif//guard
