#include <deco/types/integral.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>


template<typename T, typename Stream>
void write_type(Stream& stream)
{
	if constexpr (std::is_signed_v<T>) {
		auto value = std::numeric_limits<T>::min();
		gs::serializer(stream, value);
	}
	else {
		auto value = std::numeric_limits<T>::max();
		gs::serializer(stream, value);
	}
};

template<typename T, typename I>
void read_type(deco::InputStream<I>& stream)
{
	T value;
	gs::serializer(stream, value);

	if constexpr (std::is_signed_v<T>)
		assert(value == std::numeric_limits<T>::min());
	else
		assert(value == std::numeric_limits<T>::max());
};


int main()
{
	// write
	{
		deco::OutputStream_indent stream;
		const auto serialize = [&stream](auto&& t) {
			deco::serialize(stream, t);
		};

		write_type<char>(stream);
		write_type<unsigned char>(stream);
		write_type<short>(stream);
		write_type<unsigned short>(stream);
		write_type<int>(stream);
		write_type<unsigned int>(stream);
		write_type<long>(stream);
		write_type<unsigned long>(stream);
		write_type<long long>(stream);
		write_type<unsigned long long>(stream);

		std::ofstream os("out.deco", std::ios::binary);
		os << stream.str;
	}

	// read
	{
		auto file = std::ifstream("out.deco", std::ios::binary);
		std::string file_str{
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>() };

		std::cout << file_str;

		auto stream = deco::make_InputStream(file_str.cbegin());

		read_type<char>(stream);
		read_type<unsigned char>(stream);
		read_type<short>(stream);
		read_type<unsigned short>(stream);
		read_type<int>(stream);
		read_type<unsigned int>(stream);
		read_type<long>(stream);
		read_type<unsigned long>(stream);
		read_type<long long>(stream);
		read_type<unsigned long long>(stream);
	}
}