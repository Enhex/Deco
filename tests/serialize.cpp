#include <InputStream.h>
#include <OutputStream.h>
#include <NVP.h>

#include <cassert>
#include <fstream>
#include <iostream>

using namespace std;

int main()
{
	// values
	constexpr char c_val = 'c';
	constexpr int i_val = 2;
	constexpr float f_val = 3.25;
	constexpr string_view s_val = "an entry"sv;
	const vector<int> v_val{ 0,1,2,3,4,10 };

	// variables
	char c = c_val;
	int i = i_val;
	float f = f_val;
	auto s = string(s_val);
	auto v = v_val;

	// write
	{
		deco::OutputStream stream;

		gs::serialize(stream, c);
		stream.begin_set("some set");
			gs::serialize(stream, i);
			gs::serialize(stream, f);
			stream.begin_set("another set");
				gs::serialize(stream, s);
			stream.end_set();
		stream.end_set();

		gs::serialize(stream, deco::make_NVP("vec", v));

		ofstream os("out.deco", ios::binary);
		os << stream.stream.str();
	}

	// reset
	c = '0';
	i = 0;
	f = 0;
	s = "";
	v.clear();

	// read
	{
		deco::InputStream is({
			istreambuf_iterator<char>(ifstream("out.deco", ios::binary)),
			istreambuf_iterator<char>()
		});

		cout << is.str;

		std::string set_name;	// dummy

		gs::serialize(is, c);
			gs::serialize(is, set_name);
			gs::serialize(is, i);
			gs::serialize(is, f);
				gs::serialize(is, set_name);
					gs::serialize(is, s);
				is.parse_entry();		// set end
			is.parse_entry();			// set end
		gs::serialize(is, set_name);	// NVP name
		gs::serialize(is, v);

		// verify reading the same values written
		assert(c == c_val);
		assert(i == i_val);
		assert(f == f_val);
		assert(s == s_val);
		assert(v == v_val);
	}
}