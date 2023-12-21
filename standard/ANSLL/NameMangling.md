# 1. Name Mangling

All ANSLL functions should be mangled using the rules that follow unless
specified to use C linkage.

## a. ANSLL Mangling

### Special Characters

The name is ASCII encoded, meaning the following rule must be used in each
identifier. The following characters are not special:

- `A-Z`, `a-z` Latin alphabet
- `0-9` Arabic numerals
- `_`

Any other character is special and should be encoded as the following:

- Start with `@`
- Base 16 length of base 16 code point
- Base 16 hexadecimal code point

The "code point" is the code point itself and *not* its binary representation.

For example `¡` inverted exclamation point is encoded as `@2A0`,
and `ϑ` Greek theta is encoded as `@403D1`.

### Mangled Function Name

Then the name of a function is comprised of the following:

- Start with `_A`
- `F` for function
- `$`
- Module name with special characters encoded where `::` is encoded as a single `?`
- `$`
- Function name with special characters encoded
- `$`
- `P` for parameters
- `$`

From here each parameter's unit is printed as the following:

- Unit name with special characters encoded (e.g. `s` or `Hz`), if the unit is `none` then this is empty
- `$`

There may be zero or many of those unit-`$` pairs.

## b. C Function Name

If a function is specified to have C linkage, it should have a function name
that is its raw function name. Note that this doesn't mean the identifier must
be allowed in C, but means that any ASCII character is allowed. However,
non-ASCII characters are not allowed since they would have to be mangled
somehow.

