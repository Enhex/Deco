#ifndef deco_OutputStream_h
#define deco_OutputStream_h

#include "deco.h"
#include <Generic Serialization/Header.h>
#include <sstream>
#include <string>
#include <vector>

namespace deco
{
	/*TODO:
	- generic type save(T to str) / load(str to T) / serialize(automatically choose based on input/output) functions (build on top of a separate library)
	- file save/load
	*/

	// Used to construct deco document string on the fly
	/*TODO
	handle it like General Serialization write type - have a gs::read() specialization that takes a value and writes it as an entry
	*/
	struct OutputStream
	{
		std::stringstream stream;

		void entry(const std::string& content)
		{
			indent();
			stream << content << '\n';
		}

		void begin_set(std::string&& content)
		{
			entry(content += ':');
			++indent_level;
		}
		void begin_set(const std::string& content)
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
				stream << '\t';
		}
	};
}


namespace gs
{
	template<>
	struct is_output<deco::OutputStream> : std::true_type {};


	//automatically provide default serialization implementation for arithmetic & array of arithmetic types
	template<typename T>
	typename std::enable_if_t<std::is_arithmetic_v<T>>
		write(deco::OutputStream& stream, const T& value) {

		std::stringstream ss;
		ss << value;
		stream.entry(ss.str());
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