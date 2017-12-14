#include <deco/types/floating_point.h>

#include <gs/serializer.h>

#include <cassert>
#include <fstream>
#include <iostream>

constexpr auto floating_test_value = 123.125;	// 0.125 shouldn't have floating point precision error

int main()
{
	// write
	{
		deco::OutputStream_indent stream;
		const auto serialize = [&stream](auto&& t) {
			deco::serialize(stream, t);
		};

		{
			const float value = floating_test_value;
			gs::serializer(stream, value);
		}
		{
			const double value = floating_test_value;
			gs::serializer(stream, value);
		}
		{
			const long double value = floating_test_value;
			gs::serializer(stream, value);
		}

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

		{
			float value;
			gs::serializer(stream, value);
			assert(value == floating_test_value);
		}
		{
			double value;
			gs::serializer(stream, value);
			assert(value == floating_test_value);
		}
		{
			long double value;
			gs::serializer(stream, value);
			assert(value == floating_test_value);
		}
	}
}