#include <Deco.h>
#include <rang.hpp>
#include <string>

void print(const deco::Entry& entry, size_t& depth);
void print(const deco::Entry& entry) {
	size_t depth = 0;
	print(entry, depth);
}

void print(const deco::Entry& entry, size_t& depth) {
	using namespace std;

	auto indent = [&depth]() {
		for (size_t i = 0; i < depth; ++i)
			cout << '\t';
	};
	indent();

	cout << rang::bg::blue << entry.content << rang::bg::black;

	if (!entry.entries.empty())
	{
		cout << rang::fgB::green << ":" << rang::fg::gray << '\n';

		++depth;
		for (const auto& child : entry.entries)
			print(child, depth);
		--depth;

		indent();
		cout << rang::fgB::red << ":" << rang::fg::gray;
	}

	cout << '\n';
}


int main()
{
	using namespace std;

	string sample{
		"hello\n"
		"world!:\n"
		"	from\n"
		"	Deco:\n"
		"		Delimiter\n"
		"		collision\n"
		"		Free\n"
		"		Format\n"
		"	:\n"
		":\n"
		"content ending with structure delimiter:'\n"
		"'		 content starting with whitespace\n"
		": structure : delimiter : and ' content ' delimiter ' as : content ' without : escaping\n"
		"':\n"
		"	anonymous\n"
		"	set\n"
		":\n"
		"no:\n"
		"need:\n"
		"to:\n"
		"indent\n"
		":\n"
		":\n"
		":\n"
	};

	cout << rang::fg::yellow << "sample:" << rang::fg::gray << endl;
	cout << sample << endl << endl;

	// make sure ending with entry delimiter
	if (sample.back() != deco::entry_delimiter) {
		sample.push_back(deco::entry_delimiter);
	}

	auto entries = deco::parse(sample.begin(), sample.end());

	cout << rang::fg::yellow << "result:" << rang::fg::gray << endl;
	for (const auto& entry : entries)
		print(entry);

	cout << endl;
}