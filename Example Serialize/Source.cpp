#include <Serialization.h>

#include <cassert>
#include <fstream>
#include <rang.hpp>

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
		deco::Document doc;

		deco::serialize(doc, c);
		doc.begin_set("some set");
			deco::serialize(doc, i);
			deco::serialize(doc, f);
			doc.begin_set("another set");
				deco::serialize(doc, s);
			doc.end_set();
		doc.end_set();

		deco::serialize(doc, deco::make_NVP("vec", v));

		ofstream os("out.deco", ios::binary);
		os << doc.stream.str();
	}

	// reset
	c = '0';
	i = 0;
	f = 0;
	s = "";
	v.clear();

	// read
	{
		const string str(
			istreambuf_iterator<char>(ifstream("out.deco", ios::binary)),
			istreambuf_iterator<char>());

		cout << str;

		const auto entries = deco::parse(str.begin(), str.end());

		from_entry(c, entries[0]);
		from_entry(i, entries[1].entries[0]);
		from_entry(f, entries[1].entries[1]);
		from_entry(s, entries[1].entries[2].entries[0]);
		from_entry(v, entries[2]);

		// verify reading the same values written
		assert(c == c_val);
		assert(i == i_val);
		assert(f == f_val);
		assert(s == s_val);
		assert(v == v_val);
	}
}