#include <deco/NVP.h>
#include <deco/arithmetic.h>
#include <deco/vector.h>

#include <fstream>
#include <iostream>

using namespace std;

struct A {
	int i = 2;
	string s = "an entry";
};

namespace gs
{
	template<typename Stream>
	void serialize(Serializer<Stream>& serializer, A& value) // shouldn't be const to allow reading
	{
		using namespace deco;
		serializer(
			make_NVP("i", value.i),
			make_NVP("s", value.s));
	}
}


struct B {
	A a;
	vector<int> v{ 0,1,2,3,4,10 };
	vector<A> va{ A(), A(), A() };
	float f = 3.25;
};

namespace gs
{
	template<typename Stream>
	void serialize(Serializer<Stream>& serializer, B& value) {
		using namespace deco;
		serializer(
			make_set("a", value.a),
			make_set("v", value.v),
			make_set("va", value.va),
			make_NVP("f", value.f));
	}
}


int main()
{
	// write
	{
		B b;
		int i = 5;

		ofstream os("out.deco", ios::binary);
		deco::OutputStream_Indent stream;
		auto serializer = gs::make_serializer(stream);

		serializer(b, i);

		os << stream.str;
	}

	// read
	{
		B b;
		b.f = 0;
		b.v.clear();
		b.a.i = 0;
		b.a.s = '0';

		int i = 0;

		string file_str{
			istreambuf_iterator<char>(ifstream("out.deco", ios::binary)),
			istreambuf_iterator<char>()};

		cout << file_str;

		deco::InputStream stream(file_str.cbegin());
		auto serializer = gs::make_serializer(stream);

		serializer(b, i);
	}
}