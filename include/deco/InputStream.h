#ifndef deco_InputStream_h
#define deco_InputStream_h

#pragma warning(push, 0) // disable external warnings
#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/numeric.hpp>
#pragma warning(pop)

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

		constexpr Entry peek_entry() {
			return deco::peek_entry(position);
		}

		constexpr bool peek_list_end() {
			return deco::peek_list_end(position);
		}
	};

	template<typename Iterator>
	constexpr auto make_InputStream(Iterator&& iter)
	{
		return InputStream<Iterator>{std::forward<Iterator>(iter)};
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
}

namespace gs
{
	template<typename I> struct is_input<deco::InputStream<I>> : std::true_type {};

	template<typename I> struct is_deco<deco::InputStream<I>> : std::true_type {};

	template<typename T>
	constexpr auto is_deco_input_v = is_deco_v<T> && is_input_v<T>;


}

namespace deco
{
	// serialize input deco
	template<typename Stream, typename T> constexpr
		std::enable_if_t<gs::is_deco_input_v<Stream>>
		serialize(Stream& stream, T&& value)
	{
		read(stream, std::forward<T>(value));
	}
	
	// skip entry without parsing
	template<typename Stream> constexpr
		std::enable_if_t<gs::is_deco_input_v<Stream>>
		serialize(Stream& stream, const deco::skip_t&)
	{
		stream.parse_entry();
	}
}

#endif//guard
