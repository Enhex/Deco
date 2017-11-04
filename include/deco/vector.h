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
		serialize(Stream& stream, std::vector<T>& value) {
		static_assert(false, "std::vector must be serialized as a set entry");
	}

	// allow vector as set entry
	template<typename Stream, typename T>
	typename std::enable_if_t<std::is_base_of_v<deco::OutputStream, Stream>>
		write(Stream& stream, const deco::set_t<std::vector<T>>& nvp)
	{
		stream.begin_set(nvp.name);
		write(stream, nvp.value);
		stream.end_set();
	}

	template<typename Stream, typename T>
	typename std::enable_if_t<std::is_base_of_v<deco::OutputStream, Stream>>
		write(Stream& stream, std::vector<T>& value)
	{
		for (auto& e : value)
			serialize(stream, e);
	}


	template<typename T>
	void read(deco::InputStream& stream, std::vector<T>& value)
	{
		//NOTE: set-entry content should've been read already, now reading children
		while (!stream.peek_set_end())
			serialize(stream, value.emplace_back());
		//NOTE: set end will be skipped by the caller
	}
}

#endif//guard