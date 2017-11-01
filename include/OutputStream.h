#ifndef deco_OutputStream_h
#define deco_OutputStream_h

#include "Traits.h"
#include "deco.h"
#include <gs/Core.h>
#include <string>
#include <vector>

namespace deco
{
	struct OutputStream
	{
		std::string str;

		void entry(const std::string& content)
		{
			indent();
			(str += content) += '\n';
		}

		void begin_set(std::string&& content)
		{
			entry(content += ':');
			++indent_level;
		}
		void begin_set(const std::string_view& content)
		{
			begin_set(std::string(content));
		}

		void end_set()
		{
			--indent_level;
			entry(std::string(":"));
		}

	protected:
		unsigned indent_level = 0;

		void indent()
		{
			for (unsigned n = 0; n < indent_level; ++n)
				str += '\t';
		}
	};
}


namespace gs
{
	template<>
	struct is_output<deco::OutputStream> : std::true_type {};

	template<>
	struct is_deco<deco::OutputStream> : std::true_type {};


	// serialize output deco
	template<typename Stream, typename T>
	typename std::enable_if_t<
		is_deco_v<Stream> &&
		is_output_v<Stream>
	>
	serialize(Stream& stream, T& value)
	{
		write(stream, value);
	}

	//automatically provide default serialization implementation for arithmetic types
	template<typename T>
	typename std::enable_if_t<std::is_arithmetic_v<T>>
		write(deco::OutputStream& stream, const T& value) {
		stream.entry(std::to_string(value));
	}

	void write(deco::OutputStream& stream, const std::string& value) {
		stream.entry(value);
	}

	template<typename T>
	void write(deco::OutputStream& stream, const std::vector<T>& value)
	{
		for (const auto& e : value)
			serialize(stream, e);
	}
}

#endif//guard