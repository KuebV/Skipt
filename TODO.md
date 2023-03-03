# TODO : Skipt

## Compile.cpp:
- Add all data-types in Token::dataTypes
- Migrate parseString & parseArray to using String::Substring
- Implement While Conditional Statements
  - We will not be adding for-statements in Skipt!
    - **Why?**
    - It's just simply not worth it, a while look can do all the things that a for loop can do
    ```cpp
    for (int i = 0; i < 10; i++) // Do something
    or
    
    int i = 0;
    while (i != 10) // Do something
    ```
    - This does mean however, we need to have a way to iterate or parse arrays! Possible Function of:
    
    ```ref Array::At(arrayVariable, int index) >> ValueAtIndex```

## Functions.cpp:
- Complete Math, String, Array, Internal
- Internal may contain functions that allow Skipt to interact directly with the Skipt Compiler?

## Conditional::Operators.h:
- Finish operators such as ++, +=, --, -=, %
- ^ Must be compatable with Arrays!

## Properties!
- Find a way, I don't care how. But please find a way to read from the compiler.properties file! This should be a clean implementation if possible!

## Oopsie woopsie! 
- Skipt can't process negative numbers!
- Too bad!