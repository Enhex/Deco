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

	// Used to construct deco document string
	struct Document
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

	template<typename T>
	void serialize(Document& doc, const NVP<T>& nvp)
	{
		doc.begin_set(nvp.name);
		serialize(doc, nvp.value);
		doc.end_set();
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

	template<typename T>
	void serialize(Document& doc, const CNVP<T>& nvp)
	{
		std::stringstream ss;
		ss << nvp.name << ": " << nvp.value;
		serialize(doc, ss.str());
	}

	//automatically provide default serialization implementation for arithmetic & array of arithmetic types
	template<typename T>
	typename std::enable_if_t<std::is_arithmetic_v<T>>
	serialize(Document& doc, const T& value) {

		std::stringstream ss;
		ss << value;
		doc.entry(ss.str());
	}

	void serialize(Document& doc, const std::string& value) {
		doc.entry(value);
	}

	template<typename T>
	void serialize(Document& doc, const std::vector<T>& value)
	{
		for (const auto& e : value)
			serialize(doc, e);
	}


	void from_entry(char& value, const deco::Entry& entry)
	{
		value = entry.content[0];
	}

	void from_entry(int& value, const deco::Entry& entry)
	{
		value = stoi(std::string(entry.content)); // no string_view/iterators support
	}

	void from_entry(float& value, const deco::Entry& entry)
	{
		value = stof(std::string(entry.content)); // no string_view/iterators support
	}

	void from_entry(std::string& value, const deco::Entry& entry)
	{
		value = entry.content;
	}

	template<typename T>
	void from_entry(std::vector<T>& value, const deco::Entry& entry)
	{
		T temp;

		for (const auto& child : entry.entries) {
			from_entry(temp, child);
			value.emplace_back(temp);
		}
	}
}

#endif//guard