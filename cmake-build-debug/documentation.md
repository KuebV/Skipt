# Skipt Practices & Documentation
## Skipt is a interpreted language with the main goal to having a relatively easy to use syntax and having the same performance as other languages. 
Additionally, the language is designed to have the user have complete control of their variables and program
### Data-Types
- int
- double
- string
- bool
### Array Types
- int[]
- double[]
- string[]
- bool[]

**Example Variable & Output**
```
# This indicates a comment

string name = "Robert Thompson"
int age = 19;
# ^ Skipt doesn't care whether or not you end your lines of code with semi-colons or not, they are processed the same

> name
> 
> age
```

### Input & Output
Skipt contains two types of outputs, one is the traditional output, corresponding to the `>` character, this will output any variable that is given

A non-traditional output can be done via `ref System::out()` this contains output formatting. An example of this can be shown below:

```
# Traditional Output
int favoriteNumber = 42;
> "Your favorite number is "
> favoriteNumber

# Non Traditional Output
int favoriteNumber = 42;
ref System::out("Your favorite number is {0}" , favoriteNumber)
```
Skipt only contains 1 type of input, this corresponds to the `<` character, followed shortly by a variable. Skipt will convert the variable to it's specified type

### Conditional Statements

Skipt is very direct with conditional statements, you can only have one condition in each statement. There are only 2 conditional statments, if, and while. For loops do not exist for the sole purpose of because a while loop can do the exact same thing, with some extra coding.

**If-Statements**

```cpp
bool isAdult;
int age = 0;
if (age >= 18)
{
    isAdult = true
    > "You are an adult!"
}
else
{
    isAdult = false
    > "You are not an adult!"
}

```
With If-Statements, there is one key formatting you must keep in mind. There must be a starting bracket the line after the conditional statement is defined. Here are a few examples:

```cpp
# Valid
if (x == y)
{
    # Do something
}

# Invalid
if (x == y){
    # [Error] | [Compile.cpp] [Starting Conditional Statement Bracket]: Conditional Start Statement Bracket is not expected on this line!
}

# Invalid
if (x == y)
    # This will result in nothing happening, the program may unexpectedly quit
```

**While-Loops**
```cpp
int i = 0
while (i < 10)
{
    > i
    > ", "
    
    i = i + 1
}

# Output:
# 0, 1.000000, 2.000000, 3.000000, 4.000000, 5.000000, 6.000000, 7.000000, 8.000000, 9.000000,
```

*As seen above, even though we defined i as an integer, it displayed as if it was a double. This is because of how Skipt calcuates and stores variables. When a variable is defined as an integer, the program will register it as an integer, but in calculations, and processes, it is calculated as if it's a double. This is done to reduce the amount of conversions that the compiler has to do from one data-type to another.*

### Operators

Operators cover the following symbols: +=, -=, /=, *=, %=. These can be used to modify variables, and are very easy to use

**Valid Examples**
```cpp
int numOne = 1;
int numTwo = 2;

numOne += numTwo;
> numOne
# // Output -> 3

numOne -= numTwo;
> numOne
# // Output -> 1

numOne *= 5;
> numOne
# // Output -> 5

numOne /= 5;
> numOne
# // Output -> 1

numOne = 10;
numOne %= 2;
> numOne
# // Output -> 0
```

**Invalid Examples**
```cpp
int numOne = 1;
int numTwo = 2;
int numThree = 0;

# // Invalid, you cannot assign an operation to another variable
numThree = numOne += numTwo;

if (numOne %= 2 == 0)
{
    # // Invalid, you cannot include any increment, decrement, or any of the sort operators inside a conditional statement
}
```