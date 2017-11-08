#ifndef deco_set_h
#define deco_set_h

#include "InputStream.h"
#include "OutputStream.h"
#include <gs/serializer.h>
#include <string_view>

namespace deco
{
	// Wrapper type to serialize another type inside a set
	template<typename T>
	struct set_t
	{
		std::string& name;
		T& value;
	};

	//NOTE: class template argument deduction not supported yet
	template<typename T>
	constexpr auto make_set(std::string& name, T& value)
	{
		return set_t<T>{name, value};
	}

	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const set_t<T>& nvp)
	{
		stream.begin_set(nvp.name);
		gs::serializer(stream, nvp.value);
		stream.end_set();
	}

	template<typename T> constexpr
		void read(InputStream& stream, set_t<T>& nvp)
	{
		gs::serializer(stream, nvp.name);	// read entry name
		gs::serializer(stream, nvp.value);	// read child entry
		stream.parse_entry();				// skip set end
	}




	// optimized for anonymous set case
	template<typename T>
	struct set_anonymous_t
	{
		T& value;
	};

	//NOTE: class template argument deduction not supported yet
	template<typename T>
	constexpr auto make_set(T& value)
	{
		return set_anonymous_t<T>{value};
	}

	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const set_anonymous_t<T>& nvp)
	{
		stream.begin_anonymous_set();
		gs::serializer(stream, nvp.value);
		stream.end_set();
	}

	template<typename T> constexpr
		void read(InputStream& stream, set_anonymous_t<T>& nvp)
	{
		gs::serializer(stream, skip);		// skip set entry name
		gs::serializer(stream, nvp.value);	// read child entry
		stream.parse_entry();				// skip set end
	}




	// version that doesn't read a name
	template<typename T>
	struct set_ignore_name_t
	{
		const std::string_view name;
		T& value;
	};

	//NOTE: class template argument deduction not supported yet
	template<typename T>
	constexpr auto make_set(std::string_view&& name, T& value)
	{
		return set_ignore_name_t<T>{std::forward<std::string_view>(name), value};
	}

	template<typename Stream, typename T> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
	write(Stream& stream, const set_ignore_name_t<T>& nvp)
	{
		stream.begin_set(nvp.name);
		gs::serializer(stream, nvp.value);
		stream.end_set();
	}

	template<typename T> constexpr
	void read(InputStream& stream, set_ignore_name_t<T>& nvp)
	{
		gs::serializer(stream, skip);		// skip set entry name
		gs::serializer(stream, nvp.value);	// read child entry
		stream.parse_entry();				// skip set end
	}
}

#endif//guard