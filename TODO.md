# TODO : Skipt

## Compile.cpp:
- Add all data-types in Token::dataTypes
- Migrate parseString to using String::Substring

## Functions.cpp:
- Complete Math, String, Array, Internal
- Internal may contain functions that allow Skipt to interact directly with the Skipt Compiler?

## Conditional::Operators.h:
- Finish operators such as ++, +=, --, -=, %
- ^ Must be compatable with Arrays!

## Change how Functions handle tokens
- Rather than having Functions.cpp convert the tokens into their values, the actual function should handle it.
- This is superior because it allows strings to contains commas