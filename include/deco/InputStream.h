#ifndef deco_InputStream_h
#define deco_InputStream_h

#include <boost/spirit/home/x3.hpp>

// used to fix boost spirit karma signbit error bug
namespace boost::spirit::detail {
	using boost::spirit::x3::signbit;
}

#include <boost/spirit/home/x3/numeric.hpp>

#include "Deco.h"
#include "traits.h"
#include <gs/traits.h>

namespace deco
{
	template<typename Iterator>
	struct InputStream
	{
		InputStream(Iterator begin) noexcept : position(begin) {}

		Iterator position;

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

	template<typename Iterator>
	constexpr auto make_InputStream(Iterator&& iter)
	{
		return InputStream<Iterator>{std::forward<Iterator>(iter)};
	}

	template<typename Stream, typename T> constexpr
	void read_elements(Stream&& stream, T&& value)
	{
		//NOTE: set-entry content should've been read already, now reading children
		while (!stream.peek_set_end())
			gs::serialize(stream, value.emplace_back());
		//NOTE: set end will be skipped by the caller
	}

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

	template<typename T, typename I> constexpr
	void read(InputStream<I>& stream, T& value)
	{
		read(stream.parse_entry(), value);
	}
}

namespace gs
{
	template<typename I> struct is_input<deco::InputStream<I>> : std::true_type {};

	template<typename I> struct is_deco<deco::InputStream<I>> : std::true_type {};

	template<typename T>
	constexpr auto is_deco_input_v = is_deco_v<T> && is_input_v<T>;


	// serialize input deco
	template<typename Stream, typename T> constexpr
	std::enable_if_t<is_deco_input_v<Stream>>
	serialize(Stream& stream, T& value)
	{
		deco::read(stream, value);
	}


	// skip entry without parsing
	template<typename Stream> constexpr
	std::enable_if_t<is_deco_input_v<Stream>>
	serialize(Stream& stream, const deco::skip_t&)
	{
		stream.parse_entry();
	}

	constexpr void serialize(deco::Entry& entry, const deco::skip_t&)
	{
	}
}

#endif//guard
