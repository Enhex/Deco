#pragma once

#include <vector>

serialize(Stream& f, std::vector& value)
{
	f.begin_set(name);

	for (auto& e : value)
		serialize(e);

	f.end_set();
}