# Skipt

## Important Issues
- Peeking causes an error
  - What should happen:
    - The program runs as normal, and skips the error when there's no more lines
  - What actually happens:
    - Skipt throws an error

- Incomplete Functions
- Documentation

## Medium Issues
```cpp
int x = 10;
int y;

y = x;
// What should happen:
// y inherits the value of x

// What actually happens:
// y inherits an empty value
```

## Low Issues
