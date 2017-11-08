#ifndef Deco_h
#define Deco_h

#include <array>
#include <limits>
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


	constexpr auto make_whitespace_jump_table() noexcept
	{
		std::array<unsigned char, std::numeric_limits<unsigned char>::max()> table{ 0 };
		table['\t'] = 1;
		table[' '] = 1;
		return table;
	}
	constexpr auto whitespace_jump_table = make_whitespace_jump_table();

	// skip whitespace
	template <typename Iterator>
	constexpr void skip_whitespace(Iterator& current)
	{
		while (whitespace_jump_table[*current])
			++current;
	}
	template <typename Iterator>
	constexpr auto skip_whitespace(Iterator&& current)
	{
		while (whitespace_jump_table[*current])
			++current;
		return current;
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

	// parse next entry
	template <typename Iterator>
	constexpr Entry parse_entry(Iterator& current)
	{
		Entry entry;

		// expecting tabs for indentation
		skip_whitespace(current);

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


	template <typename Iterator>
	constexpr EntryObject parse_object(Iterator& current)
	{
		return parse_object(current, parse_entry(current));
	}

	template <typename Iterator>
	constexpr EntryObject parse_object(Iterator& current, const Entry& entry)
	{
		switch (entry.type)
		{
		case Entry::entry:
			return { entry.content, {} };

		case Entry::set:
		{
			auto object = EntryObject{ entry.content, {} };

			// Keep consuming entries until reaching the set end. Child sets will consume their own ends, so current set won't run into their ends.
			for(auto child_entry = parse_entry(current); child_entry.type != Entry::set_end; child_entry = parse_entry(current))
				object.entries.emplace_back(parse_object(current, child_entry));
			
			return object;
		}
		}

		throw std::invalid_argument("Unexpected set end");
	}

	template <typename Iterator>
	constexpr auto parse(Iterator current, const Iterator last)
	{
		std::vector<EntryObject> objects;

		while (current != last) {
			objects.emplace_back(parse_object(current));
		}

		return objects;
	}


	template <typename Iterator>
	constexpr Entry peek_entry(Iterator current)
	{
		return parse_entry(current);
	}


	template <typename Iterator>
	constexpr bool peek_set_end(Iterator current)
	{
		skip_whitespace(current);

		// must be structure delimiter followed by entry delimiter
		if (*current == structure_delimiter &&
			*(++current) == entry_delimiter)
			return true;

		return false;
	}
}

#endif//guard