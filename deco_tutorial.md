# Deco Tutorial

### Delimiters

- `entry delimiter`     = `\n` (newline)
- `structure delimiter` = `:`
- `content delimiter`   = `'`

### Format structure

#### Entry
Each entry is separated by an `entry delimiter`.

```
first entry
second entry
third entry
```

The whitespace (spaces and tabs) at the beginning of the entry is ignored, and not part of the entry's content.

Example of an entry starting with whitespace:
```
    a b c
```
The content of the entry is `a b c`.

The `content delimiter` is used to include whitespace at the beginning of the content.

For example:
```
'    a b c
```
The content of the entry is <code>&nbsp;&nbsp;&nbsp;&nbsp;a b c</code>.


#### List Entry

A list entry can contain child entries, including other list entries.
A list entry is an entry that ends with the `structure delimiter` before the `entry delimiter`.
A list entry's content is called the list's name.

A list end entry is an entry that only contains, after skipping whitespace, a `structure delimiter` before an `entry delimiter`.


```
name:
    child entry
    child list entry:
        child entry
    :
:
```

The content of the root list entry is `name`.

To end a regular entry with a `structure delimiter` character, the `content delimiter` can be used:
```
not a list:'
```
The content of the entry is `not a list:`.

##### Anonymous List Entry

A list entry can be created with empty content by using a `content delimiter` before the `structure delimiter`.
The `content delimiter` isn't part of the content, and it prevents the entry from being interpreted as a list end.

```
':
    child entry
:
```
