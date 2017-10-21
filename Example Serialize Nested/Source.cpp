#include <Serialization.h>

#include <fstream>
#include <iostream>

using namespace std;

struct A {
	int i = 2;
	string s = "an entry";
};

namespace deco
{
	template<typename Stream>
	void serialize(Stream& doc, const A& value) {
		serialize(doc, make_CNVP("i", value.i));
		serialize(doc, make_CNVP("s", value.s));
	}
}


struct B {
	A a;
	vector<int> v{ 0,1,2,3,4,10 };
	float f = 3.25;
};

namespace deco
{
	template<typename Stream>
	void serialize(Stream& doc, const B& value) {
		serialize(doc, make_NVP("a", value.a));
		serialize(doc, make_NVP("v", value.v));
		serialize(doc, make_CNVP("f", value.f));
	}
}


int main()
{
	// write
	{
		B b;

		deco::Document doc;
		deco::serialize(doc, b);

		ofstream os("out.deco", ios::binary);
		os << doc.stream.str();
	}

	// read
	{
		B b;
		b.f = 0;
		b.v.clear();
		b.a.i = 0;
		b.a.s = '0';

		const string str(
			istreambuf_iterator<char>(ifstream("out.deco", ios::binary)),
			istreambuf_iterator<char>());

		cout << str;

		const auto entries = deco::parse(str.begin(), str.end());

		/*TODO
		provide "deco::parse_next", so entries can be converted into objects on the fly, instead of being saved into a temporary structure.
		stream.
		- need to be able to read set children until reaching the set end
		*/

		//deco::serialize(str, b);

		//from_entry(b, entries[0]);
	}
}