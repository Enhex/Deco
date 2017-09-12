#include <Deco.h>

#include <fstream>
#include <rang.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <string_view>

using namespace std;

struct DecoDocument
{
	stringstream stream;

	template<typename T>
	void entry(const T& content)
	{
		indent();
		stream << content << '\n';
	}

	void begin_set(string&& content)
	{
		entry(content += ':');
		++indent_level;
	}

	void end_set()
	{
		--indent_level;
		entry(string(":"));
	}

protected:
	unsigned indent_level = 0;

	void indent()
	{
		for (unsigned n = 0; n < indent_level; ++n)
			stream << '\t';
	}
};


void from_entry(char& value, const deco::Entry& entry)
{
	value = entry.content[0];
}

void from_entry(int& value, const deco::Entry& entry)
{
	value = stoi(string(entry.content)); // no string_view support
}

void from_entry(float& value, const deco::Entry& entry)
{
	value = stof(string(entry.content));
}

void from_entry(string& value, const deco::Entry& entry)
{
	value = entry.content;
}


int main()
{
	char c = 'c';
	int x = 2;
	float y = 3.25;
	string s = "an entry";

	// write
	{
		DecoDocument doc;

		doc.entry(c);
		doc.begin_set("some set");
			doc.entry(x);
			doc.entry(y);
			doc.begin_set("another set");
				doc.entry(s);
			doc.end_set();
		doc.end_set();

		ofstream os("out.deco", ios::binary);
		os << doc.stream.str();
	}

	// reset
	c = '0';
	x = 0;
	y = 0;
	s = "";

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
	}
}