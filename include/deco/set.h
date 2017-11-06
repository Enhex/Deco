#ifndef deco_set_h
#define deco_set_h

#include "InputStream.h"
#include "OutputStream.h"
#include <string_view>

namespace deco
{
	// Wrapper type to serialize another type inside a set
	template<typename T>
	struct set_t
	{
		std::string_view name;
		T& value;
	};

	//NOTE: class template argument deduction not supported yet
	template<typename T>
	auto make_set(std::string_view&& name, T& value) {
		return set_t<T>{
			std::forward<std::string_view>(name),
				value	// want to always use T& member, so no forwarding
		};
	}


	template<typename Stream, typename T>
	typename std::enable_if_t<std::is_base_of_v<deco::OutputStream, Stream>>
	write(Stream& stream, const deco::set_t<T>& nvp)
	{
		stream.begin_set(nvp.name);
		gs::serialize(stream, nvp.value);
		stream.end_set();
	}

	template<typename T>
	void read(deco::InputStream& stream, deco::set_t<T>& nvp)
	{
		gs::serialize(stream, deco::skip);	// skip set entry name
		gs::serialize(stream, nvp.value);	// read child entry
		stream.parse_entry();			// skip set end
	}
}

#endif//guard