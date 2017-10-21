#ifndef Deco_h
#define Deco_h

#include <string_view>
#include <vector>

namespace deco
{
	// Delimiters
	constexpr char entry_delimiter		{ '\n'	};
	constexpr char structure_delimiter	{ ':'	};
	constexpr char content_delimiter	{ '\''	};

	
	/*TODO
	separate into different types of entries:
	- regular entry: only has content
	- set entry: has content and child entries
	- anonymous set entry: only has child entries
	*/
	struct EntryObject {
		std::string_view content;
		std::vector<EntryObject> entries;
	};


	/*TODO
	most likely to only be using tabs, or only be using spaces.
	also likely to be using only tabs, then only spaces.
	skip whitespace by switching between checking only for tabs and checking only for spaces
	*/
	// check if a character is whitespace
	constexpr bool tab_space(const char& c) {
		return (c == '\t' || c == ' ');
	}
	constexpr bool space_tab(const char& c) {
		return (c == ' ' || c == '\t');
	}

	// skip space first
	template <typename Iterator>
	void skip_whitespace(Iterator& current)
	{
		while (space_tab(*current))
			++current;
	}

	// Skip tab first
	template <typename Iterator>
	void skip_whitespace_tab(Iterator& current)
	{
		while (tab_space(*current))
			++current;
	}

	struct Entry
	{
		enum Type : uint_fast8_t {
			entry,	// non-set entry
			set,	// set entry
			set_end
		};

		std::string_view content;
		Type type;
	};

	// pair's bool is true if it's set end entry
	//TODO use std::optional with C++17
	template <typename Iterator>
	Entry parse_entry(Iterator& current, const Iterator last)
	{
		Entry entry;

		// expecting tabs for indentation
		skip_whitespace_tab(current);

		// skip content begin delimiter
		const bool content_begin_delimiter_found = [&]() {
			if (*current == content_delimiter) {
				++current;
				return true;
			}
			return false;
		}();

		// begin content
		const auto content_begin = current;	// inclusive

		// skip content
		while (*current != entry_delimiter)
			++current;

		Iterator content_end; // exclusive

		auto end_entry = [&]()
		{
			// end content
			entry.content = std::string_view(&*content_begin, std::distance(content_begin, content_end));
			// advance to the next entry
			++current;
		};

		// the character before the entry delimiter
		const auto& one_before_last = *(current - 1);

		// structure
		if (one_before_last == structure_delimiter)
		{
			content_end = current - 1;

			// check if set end entry
			if (!content_begin_delimiter_found &&
				content_end == content_begin/*check if content is empty*/)
				entry.type = Entry::set_end;

			// begin set
			else
			{
				entry.type = Entry::set;
				end_entry();
				return entry;
			}
		}
		else {
			entry.type = Entry::entry;

			// content end delimiter
			if (one_before_last == content_delimiter &&
				*(current - 2) == structure_delimiter)
				content_end = current - 1;

			// only entry delimiter
			else
				content_end = current;
		}

		end_entry();

		return entry;
	}


	//EntryObject parse_object(std::vector<EntryObject>& set, const Entry& entry)

	template <typename Iterator>
	EntryObject parse_object(Iterator& current, const Iterator last)
	{
		//return parse_object(current, last, parse_entry(current, last));

		auto test_entry = parse_entry(current, last);
		auto test_object = parse_object(current, last, test_entry);
		return test_object;
	}

	template <typename Iterator>
	EntryObject parse_object(Iterator& current, const Iterator last, const Entry& entry)
	{
		switch (entry.type)
		{
		case Entry::entry:
			return { entry.content, {} };

		case Entry::set:
		{
			auto object = EntryObject{ entry.content, {} };

			// Keep consuming entries until reaching the set end. Child sets will consume their own ends, so current set won't run into their ends.
			auto child_entry = parse_entry(current, last);
			while (child_entry.type != Entry::set_end) {
				object.entries.emplace_back(parse_object(current, last, child_entry));
				child_entry = parse_entry(current, last);
			}
			
			return object;
		}
		}

		throw std::invalid_argument("Unexpected set end");
	}

	template <typename Iterator>
	auto parse(Iterator current, const Iterator last)
	{
		std::vector<EntryObject> objects;
		//std::vector<EntryObject>* current_set = &objects;

		while (current != last) {
			objects.emplace_back(parse_object(current, last));
		}

		return objects;
	}
}

#endif//guard