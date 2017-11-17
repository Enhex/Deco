#ifndef deco_set_h
#define deco_set_h

#include "InputStream.h"
#include "OutputStream.h"
#include "types/string.h"	// for set name serialization
#include <string_view>

namespace deco
{
	// Wrapper type to start a set
	struct begin_set_t {
		std::string& name;
	};

	constexpr auto begin_set(std::string& name) {
		return begin_set_t{ name };
	}

	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const begin_set_t& value)
	{
		stream.begin_set(value.name);
	}

	template<typename I> constexpr
	void read(InputStream<I>& stream, begin_set_t& value)
	{
		serialize(stream, value.name);	// read set name
	}
	template<typename I> constexpr
	void read(InputStream<I>& stream, begin_set_t&& value)
	{
		read(stream, value);
	}




	// Wrapper type to start a set, and skip reading the name
	struct begin_set_ignore_name_t {
		const std::string_view name;
	};

	constexpr auto begin_set(std::string_view&& name) {
		return begin_set_ignore_name_t{ name };
	}

	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const begin_set_ignore_name_t& value)
	{
		stream.begin_set(value.name);
	}

	template<typename I> constexpr
	void read(InputStream<I>& stream, begin_set_ignore_name_t)
	{
		serialize(stream, skip);	// skip set name
	}




	// type to start an anonymous set
	struct begin_set_anonymous_t {};

	constexpr auto begin_set() {
		return begin_set_anonymous_t{};
	}

	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, begin_set_anonymous_t)
	{
		stream.begin_anonymous_set();
	}

	template<typename I> constexpr
	void read(InputStream<I>& stream, begin_set_anonymous_t)
	{
		serialize(stream, skip);	// skip set name
	}


	// type to end a set
	struct end_set_t {};
	constexpr end_set_t end_set;

	template<typename Stream> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, end_set_t)
	{
		stream.end_set();
	}

	template<typename I> constexpr
	void read(InputStream<I>& stream, end_set_t)
	{
		stream.parse_entry();	// skip set end
	}




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
		serialize(stream, nvp.value);
		stream.end_set();
	}
	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const set_t<T>&& nvp)
	{
		write(stream, nvp);
	}

	template<typename T, typename I> constexpr
		void read(InputStream<I>& stream, set_t<T>& nvp)
	{
		serialize(stream, nvp.name);	// read entry name
		serialize(stream, nvp.value);	// read child entry
		stream.parse_entry();			// skip set end
	}
	template<typename T, typename I> constexpr
		void read(InputStream<I>& stream, set_t<T>&& nvp)
	{
		read(stream, nvp);
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
		serialize(stream, nvp.value);
		stream.end_set();
	}
	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, set_ignore_name_t<T>&& nvp)
	{
		write(stream, nvp);
	}

	template<typename I, typename T> constexpr
		void read(InputStream<I>& stream, set_ignore_name_t<T>& nvp)
	{
		serialize(stream, skip);		// skip set entry name
		serialize(stream, nvp.value);	// read child entry
		stream.parse_entry();			// skip set end
	}
	template<typename I, typename T> constexpr
		void read(InputStream<I>& stream, set_ignore_name_t<T>&& nvp)
	{
		read(stream, nvp);
	}




	// anonymous set case
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
		serialize(stream, nvp.value);
		stream.end_set();
	}
	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const set_anonymous_t<T>&& nvp)
	{
		write(stream, nvp);
	}

	template<typename I, typename T> constexpr
	void read(InputStream<I>& stream, set_anonymous_t<T>& nvp)
	{
		serialize(stream, skip);		// skip set entry name
		serialize(stream, nvp.value);	// read child entry
		stream.parse_entry();			// skip set end
	}
	template<typename I, typename T> constexpr
	void read(InputStream<I>& stream, set_anonymous_t<T>&& nvp)
	{
		read(stream, nvp);
	}
}

#endif//guard