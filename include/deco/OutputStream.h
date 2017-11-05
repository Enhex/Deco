#ifndef deco_OutputStream_h
#define deco_OutputStream_h

// used to fix boost spirit karma signbit error bug
namespace boost::spirit::detail {
	using boost::spirit::x3::signbit;
}

#include <boost/spirit/include/karma_generate.hpp>
#include <boost/spirit/include/karma_real.hpp>

#include "Traits.h"
#include "Deco.h"
#include <gs/Core.h>

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

		void end_set() {
			entry(":");
		}
	};

	struct OutputStream_Indent : OutputStream
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
			begin_set(std::string(content));	// use own version for indenting
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

	// utility function for trimming trailing zeros and decimal point after using std::to_string
	auto& trim_float(std::string& str) {
		const auto pos = str.find_last_not_of('0');
		str.erase(pos + (str[pos] != '.'));	// if last character isn't a decimal point, don't delete it
		return str;
	}

	// don't use scientific notation
	template <typename T>
	struct fixed_policy : boost::spirit::karma::real_policies<T> {
		typedef boost::spirit::karma::real_policies<T> base_type;
		static int floatfield(T) { return base_type::fmtflags::fixed; }
		static unsigned int precision(T) { return 10; }
	};

	template<typename T>
	using float_fixed = boost::spirit::karma::real_generator<T, fixed_policy<T>>;

	template<typename T, typename = std::enable_if_t<std::is_floating_point_v<T>>>
	std::string to_string(const T& value)
	{
		std::string s;
		std::back_insert_iterator<std::string> sink(s);
		boost::spirit::karma::generate(sink, float_fixed<T>(), value);
		return s;
	}

	// escape content delimiters
	auto escape_content(const std::string_view content)
	{
		std::string str;

		if (content.empty())
			return str;

		// escape content starting with whitespace or content delimiter
		const auto first = content.front();
		if (first == ' ' ||
			first == '\t'||
			first == deco::content_delimiter)
			str.insert(str.begin(), deco::content_delimiter);

		str += content;

		// escape content ending with content delimiter or structure delimiter
		const auto last = content.back();
		if (last == deco::structure_delimiter ||
			last == deco::content_delimiter)
			str += deco::content_delimiter;

		return str;
	}

	// remove delimiters used to escape content
	template<bool unescape_content_begin = false>	// entry parsing already removes content begin delimiter
	void unescape_content(std::string_view& content)
	{
		// erase start content delimiter
		if constexpr (unescape_content_begin) {
			if (content.front() == content_delimiter)
				content.remove_prefix(1);
		}

		// erase end content delimiter
		if (content.back() == content_delimiter)
			content.remove_suffix(1);
	}

	template<bool unescape_content_begin = false>
	auto unescape_content(const std::string_view& ref_content)
	{
		auto content = ref_content;
		unescape_content(content);
		return content;
	}

	template<bool unescape_content_begin = false>
	void unescape_content(std::string& content)
	{
		// erase start content delimiter
		if constexpr (unescape_content_begin) {
			if (content.front() == content_delimiter)
				content.erase(0, 1);
		}

		// erase end content delimiter
		if (content.back() == content_delimiter)
			content.pop_back();
	}
}


namespace gs
{
	template<> struct is_output<deco::OutputStream> : std::true_type {};
	template<> struct is_output<deco::OutputStream_Indent> : std::true_type {};

	template<> struct is_deco<deco::OutputStream> : std::true_type {};
	template<> struct is_deco<deco::OutputStream_Indent> : std::true_type {};

	template<typename T>
	constexpr auto is_deco_output_v = is_deco_v<T> && is_output_v<T>;

	// serialize output deco
	template<typename Stream, typename T>
	typename std::enable_if_t<is_deco_output_v<Stream>>
	serialize(Stream& stream, T& value)
	{
		write(stream, value);
	}

	//automatically provide default serialization implementation for arithmetic types
	template<typename Stream, typename T>
	typename std::enable_if_t<
		std::is_base_of_v<deco::OutputStream, Stream> &&
		std::is_integral_v<T>
	>
	write(Stream& stream, const T& value) {
		stream.entry(std::to_string(value));
	}

	template<typename Stream, typename T>
	typename std::enable_if_t<
		std::is_base_of_v<deco::OutputStream, Stream> &&
		std::is_floating_point_v<T>
	>
	write(Stream& stream, const T& value) {
		stream.entry(deco::to_string(value));
		//stream.entry(deco::trim_float(std::to_string(value)));
	}
}

#endif//guard