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

### Building

The library uses [CMake](cmake.org) with [Conan](www.conan.io) package manager to manage dependencies.
For Conan to be able to automatically find the dependency packages use the following command:
```
conan remote add enhex https://api.bintray.com/conan/enhex/enhex
```

Using Conan you can consume the library as the package `Deco/master@enhex/stable` from the `enhex` remote.

If you're not using Conan to generate your project you can use conan with generators like [visual_studio](http://docs.conan.io/en/latest/reference/generators/visualstudio.html) to generate a `.props` file that you can add to your VS project, or use the [txt](http://docs.conan.io/en/latest/reference/generators/text.html) generator to get a list of the include directories you need to use.


### Using

To learn how to use the Deco library see the [documentation](documentation.md), and the [tests](tests) for examples.


### License

Deco is licensed under a modified version of Apache 2.0 license, that removes some of the conditions, including the requirement to give a copy of the license in Object form distribution.


### Note

while Deco is not absolutely "collision free",
it's only potentially needs escaping once at the start/end of an entry,
compared to other formats that need delimiters to be escaped N times.
so it's O(1) vs O(N) in a sense, which makes Deco fundamentally better.