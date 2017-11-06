#include <deco/NVP.h>
#include <deco/InputStream.h>
#include <deco/string.h>
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
	void serialize(Stream& stream, A& value /* shouldn't be const to allow reading */) {
		//serialize(stream, deco::make_NVP("i", value.i));
		//serialize(stream, deco::make_NVP("s", value.s));
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
	void serialize(Stream& stream, B& value) {
		using namespace deco;
		auto s = [&stream](auto& v) {serialize(stream, v); };
		s(make_set("a", value.a));
		s(make_set("v", value.v));
		s(make_set("va", value.va));
		s(make_NVP("f", value.f));
	}
}


int main()
{
	// write
	{
		B b;
		int i = 5;

		deco::OutputStream_Indent stream;
		gs::serialize(stream, b, i);

		ofstream os("out.deco", ios::binary);
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
		gs::serialize(stream, b, i);
	}
}