#ifndef deco_OutputStream_h
#define deco_OutputStream_h

#pragma warning(push, 0) // disable external warnings
#include <boost/spirit/home/x3.hpp>

// used to fix boost spirit karma signbit error bug
namespace boost::spirit::detail {
	using boost::spirit::x3::signbit;
}

#include <boost/spirit/include/karma_generate.hpp>
#include <boost/spirit/include/karma_real.hpp>
#pragma warning(pop)

#include "Deco.h"
#include "traits.h"
#include <gs/traits.h>

namespace deco
{
	struct OutputStream
	{
		std::string str;

		void entry(const std::string& content) {
			(str += content) += '\n';
		}

		void begin_set(std::string&& content) {
			entry(content += ':');
		}
		void begin_set(const std::string_view& content) {
			begin_set(std::string(content));
		}
		void begin_set(const char* content) {
			begin_set(std::string(content));
		}

		void begin_anonymous_set() {
			entry("':");
		}

		void end_set() {
			entry(":");
		}
	};

	// Indented output
	//NOTE: use own version of functions for indenting
	struct OutputStream_indent : OutputStream
	{
		using OutputStream::OutputStream;

		void entry(const std::string& content) {
			indent();
			OutputStream::entry(content);
		}

		void begin_set(std::string&& content) {
			entry(content += ':');
			++indent_level;
		}
		void begin_set(const std::string_view& content) {
			begin_set(std::string(content));	
		}
		void begin_set(const char* content) {
			begin_set(std::string(content));
		}

		void begin_anonymous_set() {
			entry("':");
			++indent_level;
		}

		void end_set() {
			--indent_level;
			entry(":");
		}

	protected:
		unsigned indent_level = 0;

		void indent() {
			for (unsigned n = 0; n < indent_level; ++n)
				str += '\t';
		}
	};

	// don't use scientific notation
	template <typename T>
	struct fixed_policy : boost::spirit::karma::real_policies<T> {
		typedef boost::spirit::karma::real_policies<T> base_type;
		static int floatfield(T) { return base_type::fmtflags::fixed; }
		static unsigned int precision(T) { return 10; }
	};

	template<typename T>
	using float_fixed = boost::spirit::karma::real_generator<T, fixed_policy<T>>;


	template<typename Stream, typename T> constexpr
		void write_elements(Stream&& stream, T& value)
	{
		for (auto& e : value)
			serialize(stream, e);
	}
}


namespace gs
{
	template<> struct is_output<deco::OutputStream> : std::true_type {};
	template<> struct is_output<deco::OutputStream_indent> : std::true_type {};

	template<> struct is_deco<deco::OutputStream> : std::true_type {};
	template<> struct is_deco<deco::OutputStream_indent> : std::true_type {};

	template<typename T>
	constexpr auto is_deco_output_v = is_deco_v<T> && is_output_v<T>;
}

namespace deco
{
	// serialize output deco
	// Using unqualified name lookup to find serialize(), so it should be called from within a deco namespace
	template<typename Stream, typename T> constexpr
	std::enable_if_t<gs::is_deco_output_v<Stream>>
		serialize(Stream& stream, T&& value)
	{
		write(stream, std::forward<T>(value));
	}
}

#endif//guard
