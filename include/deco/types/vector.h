#ifndef deco_std_vector_h
#define deco_std_vector_h

#include "../InputStream.h"
#include "../OutputStream.h"
#include "../set.h"
#include <gs/serializer.h>
#include <vector>

namespace deco
{
	template<typename Stream, typename T> constexpr
	std::enable_if_t<std::is_base_of_v<OutputStream, std::decay_t<Stream>>>
		write(Stream& stream, std::vector<T>& value)	//NOTE: calling gs::serialize prevents using const since it's used for both read & write
	{
		for (auto& e : value)
			gs::serialize(stream, e);
	}


	template<typename T, typename I> constexpr
	void read(InputStream<I>& stream, std::vector<T>& value)
	{
		//NOTE: set-entry content should've been read already, now reading children
		while (!stream.peek_set_end())
			gs::serialize(stream, value.emplace_back());
		//NOTE: set end will be skipped by the caller
	}
}

#endif//guard
