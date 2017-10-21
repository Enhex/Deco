#include <Serialization.h>

#include <fstream>
#include <rang.hpp>

using namespace std;

int main()
{
	char c = 'c';
	int x = 2;
	float y = 3.25;
	string s = "an entry";

	vector<int> v{0,1,2,3,4,10};

	// write
	{
		deco::Document doc;

		deco::serialize(doc, c);
		doc.begin_set("some set");
			deco::serialize(doc, x);
			deco::serialize(doc, y);
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
	x = 0;
	y = 0;
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
		from_entry(x, entries[1].entries[0]);
		from_entry(y, entries[1].entries[1]);
		from_entry(s, entries[1].entries[2].entries[0]);
		from_entry(v, entries[2]);
	}
}