#ifndef Deco_h
#define Deco_h

#include <array>
#include <limits>
#include <string_view>
#include <strong_type.h>
#include <vector>
#include <stdexcept>

namespace deco
{
	// Delimiters
	constexpr char entry_delimiter		{ '\n'	};
	constexpr char structure_delimiter	{ ':'	};
	constexpr char content_delimiter	{ '\''	};

	// Helps with Argument Dependent Lookup by being in deco namespace
	STRONG_TYPE(Content, std::string_view);
	
	struct EntryObject {
		Content content;
		std::vector<EntryObject> entries;
	};


	constexpr auto make_whitespace_jump_table() noexcept
	{
		std::array<unsigned char, std::numeric_limits<unsigned char>::max() + 1> table{ 0 };
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
			entry,	// non-list entry
			list,	// list entry
			list_end
		};

		Content content;
		Type type = entry;
	};


	// the part of the parsing after whitespace is skipped
	template <typename Iterator>
	constexpr Entry parse_entry_core(Iterator& current)
	{
		Entry entry;

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

		// in case of empty entry
		if (*current++ == entry_delimiter) {
			// end content
			entry.content = std::string_view(&*content_begin, 0);
			return entry;
		}

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

			// check if list end entry
			if (!content_begin_delimiter_found &&
				content_end == content_begin/*check if content is empty*/)
				entry.type = Entry::list_end;

			// begin list
			else
			{
				entry.type = Entry::list;
				end_entry();
				return entry;
			}
		}
		// content end delimiter
		else if (one_before_last == content_delimiter) {
			// type = entry by default
			content_end = current - 1;
		}
		// only entry delimiter
		else {
			// type = entry by default
			content_end = current;
		}

		end_entry();

		return entry;
	}

	// parse next entry
	template <typename Iterator>
	constexpr Entry parse_entry(Iterator& current)
	{
		// expecting tabs for indentation
		skip_whitespace(current);

		return parse_entry_core(current);
	}


	template <typename Iterator>
	EntryObject parse_object(Iterator& current)
	{
		return parse_object(current, parse_entry(current));
	}

	template <typename Iterator>
	EntryObject parse_object(Iterator& current, const Entry& entry)
	{
		switch (entry.type)
		{
		case Entry::entry:
			return { entry.content, {} };

		case Entry::list:
		{
			auto object = EntryObject{ entry.content, {} };

			// Keep consuming entries until reaching the list end. Child lists will consume their own ends, so current list won't run into their ends.
			for(auto child_entry = parse_entry(current); child_entry.type != Entry::list_end; child_entry = parse_entry(current))
				object.entries.emplace_back(parse_object(current, child_entry));
			
			return object;
		}
		case Entry::list_end:
			throw std::invalid_argument("Unexpected list end");
		}

		throw std::invalid_argument("Invalid entry type");
	}

	template <typename Iterator>
	auto parse(Iterator current, const Iterator last)
	{
		std::vector<EntryObject> objects;

		while (current != last) {
			objects.emplace_back(parse_object(current));
		}

		return objects;
	}


	template <typename Iterator>
	constexpr Entry peek_entry(Iterator& current)
	{
		// can keep whitespace skipping after peeking
		skip_whitespace(current);

		// can't advance over content while peeking
		Iterator current_copy = current;

		return parse_entry_core(current_copy);
	}


	template <typename Iterator>
	constexpr bool peek_list_end(Iterator& current)
	{
		// can keep whitespace skipping after peeking
		skip_whitespace(current);

		// can't advance over content while peeking
		auto ending = current;

		// must be structure delimiter followed by entry delimiter
		if (*current == structure_delimiter) {
			if(*(++current) == entry_delimiter) {
				current = ending; // restore original position
				return true;
			}
		}

		return false;
	}
}

#endif//guard