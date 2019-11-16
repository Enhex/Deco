#pragma once

#include "InputStream.h"
#include "OutputStream.h"
#include <fstream>

namespace deco
{
	// helper class for simplifying writing to file
	struct write_file
	{
		deco::OutputStream_indent stream;
		std::string path;

		write_file(std::string path) :
			path(path)
		{
		}

		~write_file()
		{
			std::ofstream os(path, std::ios::binary);
			os << stream.str;
		}
	};

	// helper class for simplifying reading from file
	struct read_file
	{
		InputStream<std::string::const_iterator> stream;
		std::string str;

		read_file(std::string const& path)
		{
			auto file = std::ifstream(path, std::ios::binary);
			str = std::string{
				std::istreambuf_iterator<char>(file),
				std::istreambuf_iterator<char>() };

			stream = make_InputStream(str.cbegin());
		}
	};
}