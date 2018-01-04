# Deco Library Documentation

## Writing serialization functions for new types

To be able to serialize a new type, you need to provide a `serialize` function for the type in the `deco` namespace.
`serialize` is used for both reading and writing.

For example:
```C++
#include <deco/types/integral.h>	// for int serialization
#include <gs/serializer.h>		// for gs::serializer
#include <fstream>

struct A {
	int x, y, z;
};

namespace deco
{
	template<typename Stream> constexpr
	void serialize(Stream& stream, A& value)
	{
		deco::serialize(stream, value.x);		// serialize single variable
		gs::serializer(stream, value.y, value.z);	// variadic template to serialize multiple variables
	}
}

int main()
{
	A a{1,2,3};

	// write
	{
		deco::OutputStream_indent stream;

		deco::serialize(stream, a);

		std::ofstream os("out.deco", std::ios::binary);
		os << stream.str;
	}

	a = {0,0,0};	// change values

	// read
	{
		auto file = std::ifstream("out.deco", std::ios::binary);
		std::string file_str{
			std::istreambuf_iterator<char>(file),
			std::istreambuf_iterator<char>() };

		auto stream = deco::make_InputStream(file_str.cbegin());

		deco::serialize(stream, a);
	}
}
```

If you need different behavior on read and on write, you can instead provide `deco::read` and `deco::write` functions.
See `deco/types/string.h` for example.


## List entry serialization

You can serialize a type inside a list by using `deco::make_list`.
For its name parameter it can accept:
- `std::string_view` or string literal, which it will only use to write.
- reference to a `std::string` variable, which it will also read the name into.
- no name argument at all, to create anonymous list.

For example:
```C++
#include <deco/list.h>
#include <deco/types/integral.h>
#include <gs/serializer.h>

struct A {
	int x, y, z;
	std::string name = "y";
};

namespace deco
{
	template<typename Stream> constexpr
	void serialize(Stream& stream, A& value)
	{
		gs::serializer(stream,
			deco::make_list("x", value.x),	// write
			deco::make_list(name, value.y),	// write and read
			deco::make_list(value.z));	// anonymous list
	}
}
```

If you want to serialize several variables into a list you can use the `deco::begin_list` and `deco::end_list` types.
`deco::begin_list` accepts the same kind of name parameters as `deco::make_list`.

```C++
#include <deco/list.h>
#include <deco/types/integral.h>
#include <gs/serializer.h>

struct A {
	int x, y, z;
};

namespace deco
{
	template<typename Stream> constexpr
	void serialize(Stream& stream, A& value)
	{
		gs::serializer(stream,
			begin_list("A"),
				value.x,
				value.y,
				value.z,
			end_list);
	}
}
```


## Containers

Serialization for C++'s standard containers is provided with the library.
Containers must be serialized as lists using `deco::make_list`, so they can know which entries are their elements.

For example:
```C++
#include <deco/list.h>
#include <deco/types/vector.h>

int main()
{
	std::vector<int> vec;

	// ...

	deco::serialize(stream, deco::make_list("vector", vec));

	// ...
}
```


## Name Value Pair

You can serialize a name value pair which are separated by a `:` into a single line using `deco::make_NVP`.
`deco::make_NVP` accepts the same kind of name parameters as `deco::make_list`.

When using NVP:

- The name must be not contain entry or structure delimiters.
- The value is automatically escaped, and is parsed as an entry.

For example:
```C++
#include <deco/NVP.h>

int main()
{
	deco::OutputStream_indent stream;
	deco::serialize(stream, deco::make_NVP("hello", "world"));
}
```
The example's NVP's content will be `hello: world`.

For types to be serializable as NVP values, they need to have `deco::to_string` function overload.


## String serialization

Serialization for `std::string` is provided in the header `deco/types/string.h`.
It will automatically escape structure and entry delimiters.


#### Unescaped string
`deco::unescaped_string`, strong type of `std::string`, will not escape delimiters when serialized.

Header: `deco/types/unescaped_string.h`


#### Multi-line string

Multi-line string can simply be described as a list of entries where each entry is a single line.
Since newline is used as the entry delimiter there's no need to explicitly use it since text editors will already display each entry in a new line.

For example:
```
string:
	this is a
	multi-line
	string
:
```

`deco::multiline_string`, strong type of `std::string`, is provided and it will automatically break down its content into seprate entries and put them back together into a single string when serialized.

It needs to be serialized as a list to group its entries, using `deco::make_list`.

Header: `deco/types/multiline_string.h`


#### Single-line string
`deco::singleline_string`, strong type of `std::string`, is used to provide compile time information that the string uses a single line only.
It does not perform any runtime checks to make sure it's true.

It has the `deco::is_single_entry` type trait, which allows it to be safely serialized more compactly in some cases (see `deco::is_single_entry`'s documentation).

Header: `deco/types/singleline_string.h`


#### Manually escaping a string

If you need to manually escape a string's content, you can use the functions `deco::escape_content` and `deco::unescape_content`, which are defined in `deco/escape.h`.

Keep in mind that the Deco parser already exclude delimiters from the entry's content, so you may not need to manually unescape it.

## Type traits

#### deco::is_single_entry

`deco::is_single_entry` type trait is used for types that can be serialized as a single entry, that means their string value must not contain entry delimiter.
It's defined in `deco/traits.h`.

`deco::is_single_entry` is used by key value containers such as `std::map` to know if the key type can be serialized as a single entry, so the key can be a list entry and the value its child, instead of serializing each in a separate list (in case the type is serialized as several entries).
if you want to make a new type serializable as a single entry key you need to provide an overload for `deco::to_string`, which is used to convert the key into a string.
