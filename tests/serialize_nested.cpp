#include <deco/NVP.h>
#include <deco/list.h>
#include <deco/types/arithmetic.h>
#include <deco/types/vector.h>

#include <gs/serializer.h>

#include <fstream>
#include <iostream>

using namespace std;

struct A {
	int i = 2;
	string s = "an entry";
};

namespace deco
{
	template<typename Stream> constexpr
	void serialize(Stream& stream, A& value) // shouldn't be const to allow reading
	{
		gs::serializer(stream,
			begin_list("a"),
				make_NVP("i", value.i),
				make_NVP("s", value.s),
			end_list);
	}
}


struct B {
	A a;
	vector<int> v{ 0,1,2,3,4,10 };
	vector<A> va{ A{}, A{}, A{} };
	float f = 3.25;
};

namespace deco
{
	template<typename Stream> constexpr
	void serialize(Stream& stream, B& value)
	{
		gs::serializer(stream,
			value.a,
			make_list("v", value.v),
			make_list("va", value.va),
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
		deco::OutputStream_indent stream;

		gs::serializer(stream, b, i);

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

		auto file = ifstream("out.deco", ios::binary);
		string file_str{
			istreambuf_iterator<char>(file),
			istreambuf_iterator<char>()};

		cout << file_str;

		auto stream = deco::make_InputStream(file_str.cbegin());

		gs::serializer(stream, b, i);
	}
}