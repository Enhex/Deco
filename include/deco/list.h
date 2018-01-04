#ifndef deco_list_h
#define deco_list_h

#include "InputStream.h"
#include "OutputStream.h"
#include "types/string.h"	// for list name serialization
#include <string_view>

namespace deco
{
	// Wrapper type to start a list
	struct begin_list_t {
		std::string& name;
	};

	constexpr auto begin_list(std::string& name) {
		return begin_list_t{ name };
	}

	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const begin_list_t& value)
	{
		stream.begin_list(value.name);
	}

	template<typename I> constexpr
	void read(InputStream<I>& stream, begin_list_t& value)
	{
		serialize(stream, value.name);	// read list name
	}
	template<typename I> constexpr
	void read(InputStream<I>& stream, begin_list_t&& value)
	{
		read(stream, value);
	}




	// Wrapper type to start a list, and skip reading the name
	struct begin_list_ignore_name_t {
		const std::string_view name;
	};

	constexpr auto begin_list(std::string_view&& name) {
		return begin_list_ignore_name_t{ name };
	}

	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const begin_list_ignore_name_t& value)
	{
		stream.begin_list(value.name);
	}

	template<typename I> constexpr
	void read(InputStream<I>& stream, begin_list_ignore_name_t)
	{
		serialize(stream, skip);	// skip list name
	}




	// type to start an anonymous list
	struct begin_list_anonymous_t {};

	constexpr auto begin_list() {
		return begin_list_anonymous_t{};
	}

	template<typename Stream> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, begin_list_anonymous_t)
	{
		stream.begin_anonymous_list();
	}

	template<typename I> constexpr
	void read(InputStream<I>& stream, begin_list_anonymous_t)
	{
		serialize(stream, skip);	// skip list name
	}


	// type to end a list
	struct end_list_t {};
	constexpr end_list_t end_list;

	template<typename Stream> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, end_list_t)
	{
		stream.end_list();
	}

	template<typename I> constexpr
	void read(InputStream<I>& stream, end_list_t)
	{
		stream.parse_entry();	// skip list end
	}




	// Wrapper type to serialize another type inside a list
	template<typename T>
	struct list_t
	{
		std::string& name;
		T& value;
	};

	//NOTE: class template argument deduction not supported yet
	template<typename T>
	constexpr auto make_list(std::string& name, T& value)
	{
		return list_t<T>{name, value};
	}

	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const list_t<T>& nvp)
	{
		stream.begin_list(nvp.name);
		serialize(stream, nvp.value);
		stream.end_list();
	}
	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const list_t<T>&& nvp)
	{
		write(stream, nvp);
	}

	template<typename T, typename I> constexpr
		void read(InputStream<I>& stream, list_t<T>& nvp)
	{
		serialize(stream, nvp.name);	// read entry name
		serialize(stream, nvp.value);	// read child entry
		stream.parse_entry();			// skip list end
	}
	template<typename T, typename I> constexpr
		void read(InputStream<I>& stream, list_t<T>&& nvp)
	{
		read(stream, nvp);
	}




	// version that doesn't read a name
	template<typename T>
	struct list_ignore_name_t
	{
		const std::string_view name;
		T& value;
	};

	//NOTE: class template argument deduction not supported yet
	template<typename T>
	constexpr auto make_list(std::string_view&& name, T& value)
	{
		return list_ignore_name_t<T>{std::forward<std::string_view>(name), value};
	}

	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const list_ignore_name_t<T>& nvp)
	{
		stream.begin_list(nvp.name);
		serialize(stream, nvp.value);
		stream.end_list();
	}
	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, list_ignore_name_t<T>&& nvp)
	{
		write(stream, nvp);
	}

	template<typename I, typename T> constexpr
		void read(InputStream<I>& stream, list_ignore_name_t<T>& nvp)
	{
		serialize(stream, skip);		// skip list entry name
		serialize(stream, nvp.value);	// read child entry
		stream.parse_entry();			// skip list end
	}
	template<typename I, typename T> constexpr
		void read(InputStream<I>& stream, list_ignore_name_t<T>&& nvp)
	{
		read(stream, nvp);
	}




	// anonymous list case
	template<typename T>
	struct list_anonymous_t
	{
		T& value;
	};

	//NOTE: class template argument deduction not supported yet
	template<typename T>
	constexpr auto make_list(T& value)
	{
		return list_anonymous_t<T>{value};
	}

	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const list_anonymous_t<T>& nvp)
	{
		stream.begin_anonymous_list();
		serialize(stream, nvp.value);
		stream.end_list();
	}
	template<typename Stream, typename T> constexpr
		std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, const list_anonymous_t<T>&& nvp)
	{
		write(stream, nvp);
	}

	template<typename I, typename T> constexpr
	void read(InputStream<I>& stream, list_anonymous_t<T>& nvp)
	{
		serialize(stream, skip);		// skip list entry name
		serialize(stream, nvp.value);	// read child entry
		stream.parse_entry();			// skip list end
	}
	template<typename I, typename T> constexpr
	void read(InputStream<I>& stream, list_anonymous_t<T>&& nvp)
	{
		read(stream, nvp);
	}
}

#endif//guard