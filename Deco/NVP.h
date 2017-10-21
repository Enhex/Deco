#ifndef deco_NVP_h
#define deco_NVP_h

#include "InputStream.h"
#include "OutputStream.h"
#include <string>


namespace deco
{
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