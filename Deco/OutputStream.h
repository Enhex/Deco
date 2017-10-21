#ifndef deco_Serialization_h
#define deco_Serialization_h

#include "deco.h"
#include <string>
#include <sstream>

#include <vector>
#include <Generic Serialization/Header.h>

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
	

	// name value pair
	template<typename T>
	struct NVP
	{
		std::string name;
		T value;
	};

	template<typename T>
	auto make_NVP(std::string&& name, T& value) {
		return NVP<T>{
			std::forward<std::string>(name),
			value	// want to always use T member, so no forwarding
		};
	}


	// Content name value pair: NVP inside the entry content, using the structure delimiter to separate between name and value, which means it can't be inside the name
	template<typename T>
	struct CNVP {
		std::string name;
		T value;
	};

	template<typename T>
	auto make_CNVP(std::string&& name, T& value) {
		return CNVP<T>{
			std::forward<std::string>(name),
			value	// want to always use T member, so no forwarding
		};
	}
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


	template<typename T>
	void write(deco::OutputStream& stream, const deco::NVP<T>& nvp)
	{
		stream.begin_set(nvp.name);
		serialize(stream, nvp.value);
		stream.end_set();
	}

	template<typename T>
	void write(deco::OutputStream& stream, const deco::CNVP<T>& nvp)
	{
		std::stringstream ss;
		ss << nvp.name << ": " << nvp.value;
		serialize(stream, ss.str());
	}
}

#endif//guard