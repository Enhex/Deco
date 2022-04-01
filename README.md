# Deco - Delimiter Collision Free Format

Deco is a human readable text format which avoids the [delimiter collision problem](https://en.wikipedia.org/wiki/Delimiter#Delimiter_collision).

Deco has minimal and simple syntax, while still being able to represent grouping and nesting.
It's very easy to read and write by humans.

For example this is how the [JSON example from Wikipedia](https://en.wikipedia.org/wiki/JSON#Example) would look like in Deco:

<table>
<tr>
  <td>
      Deco
  </td>
  <td>
      JSON
  </td>
</tr>
<tr>
<td>
<pre>
firstName: John
lastName: Smith
isAlive: true
age: 27
address:
    streetAddress: 21 2nd Street
    city: New York
    state: NY
    postalCode: 10021-3100
:
phoneNumbers:
    type: home
    number: 212 555-1234
    type: office
    number: 646 555-4567
    type: mobile
    number: 123 456-7890
:
children:
:
spouse: null
</pre>
</td>
<td>
<pre>
{
  "firstName": "John",
  "lastName": "Smith",
  "isAlive": true,
  "age": 27,
  "address": {
    "streetAddress": "21 2nd Street",
    "city": "New York",
    "state": "NY",
    "postalCode": "10021-3100"
  },
  "phoneNumbers": [
    {
      "type": "home",
      "number": "212 555-1234"
    },
    {
      "type": "office",
      "number": "646 555-4567"
    },
    {
      "type": "mobile",
      "number": "123 456-7890"
    }
  ],
  "children": [],
  "spouse": null
}
</pre>
</td>
</tr>
</table>

Since Deco is simple and doesn't have to deal with delimiter collisions it's also efficient. In a [benchmark](https://github.com/Enhex/deco-benchmark) against JSON Deco had:
- over 40% smaller file size
- over 700% faster output serialization
- over 2000% faster input parsing


To learn how to use the Deco format, see the [tutorial](deco_tutorial.md).

To learn how the Deco format works, see the [specification](delimiter%20collision%20free%20format.txt).

## Deco Library

The Deco library is header-only and uses C++17.

A Conan package is available: https://conan.io/center/deco


### Using

To learn how to use the Deco library see the [documentation](documentation.md), and the [tests](tests) for examples.


### Manual Packaging

The library uses [Premake](https://premake.github.io/) with [Conan](https://conan.io/) package manager to manage dependencies.
you need to add the following Conan packages manually using the command `conan create . enhex/stable --build=outdated` from their root directory (where the conanfile.py is located):  
https://github.com/Enhex/generic_serialization  
https://github.com/Enhex/strong_type  
  
Then use the same command to create a package for Deco.  
Using Conan you can consume the library as the package `Deco/master@enhex/stable`.


### Note

While Deco is not absolutely "collision free",
it's only potentially needs escaping once at the start/end of an entry,
compared to other formats that need delimiters to be escaped N times.
So it's O(1) vs O(N) in a sense, which makes Deco fundamentally better.
