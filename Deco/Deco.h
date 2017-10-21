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
	struct Entry {
		std::string_view content;
		std::vector<Entry> entries;
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


	// pair's bool is true if it's set end entry
	//TODO use std::optional with C++17
	template <typename Iterator>
	std::pair<bool, Entry> parse_entry(Iterator& current, const Iterator last, size_t& depth)
	{
		std::pair<bool, Entry> setEnd_entry{ false, {} };
		auto& entry = setEnd_entry.second;

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
		auto& one_before_last = *(current - 1);

		// structure
		if (one_before_last == structure_delimiter)
		{
			content_end = current - 1;

			// check if set end entry
			if (!content_begin_delimiter_found &&
				content_end == content_begin/*check if content is empty*/)
				setEnd_entry.first = true;

			// begin set
			else
			{
				end_entry();

				// parse set entries
				const auto current_depth = depth++;
				while (depth != current_depth)
				{
					auto child_entry = parse_entry(current, last, depth);
					if (child_entry.first)
						--depth;
					else
						entry.entries.emplace_back(std::move(child_entry.second));
				}

				return setEnd_entry;
			}
		}

		// content end delimiter
		else if(one_before_last == content_delimiter &&
				*(current - 2) == structure_delimiter)
			content_end = current - 1;

		// only entry delimiter
		else
			content_end = current;

		end_entry();

		return setEnd_entry;
	}

	template <typename Iterator>
	std::vector<Entry> parse(Iterator current, const Iterator last)
	{
		std::vector<Entry> entries;

		size_t depth = 0;	//TODO can get rid off by changing the way set entries are parsed?
		while (current != last) {
			entries.emplace_back(parse_entry(current, last, depth).second);
		}

		return entries;
	}
}

#endif//guard