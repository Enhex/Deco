#ifndef deco_std_vector_h
#define deco_std_vector_h

#include "InputStream.h"
#include "OutputStream.h"
#include "set.h"
#include <vector>

namespace gs
{
	// disallow vector as non-set entry
	template<typename Stream, typename T>
	typename std::enable_if_t<is_deco_output_v<Stream>>
		serialize(Serializer<Stream>& serializer, std::vector<T>& value);
}

namespace deco
{
	// allow vector as set entry
	template<typename Stream, typename T> constexpr
	typename std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(gs::Serializer<Stream>& serializer, const set_t<std::vector<T>>& nvp)
	{
		auto& stream = serializer.stream;

		stream.begin_set(nvp.name);
		write(serializer, nvp.value);
		stream.end_set();
	}

	template<typename Stream, typename T> constexpr
	typename std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(gs::Serializer<Stream>& serializer, std::vector<T>& value)
	{
		for (auto& e : value)
			serializer(e);
	}


	template<typename T> constexpr
	void read(gs::Serializer<InputStream&>& serializer, std::vector<T>& value)
	{
		//NOTE: set-entry content should've been read already, now reading children
		while (!serializer.stream.peek_set_end())
			serializer(value.emplace_back());
		//NOTE: set end will be skipped by the caller
	}
}

#endif//guard
