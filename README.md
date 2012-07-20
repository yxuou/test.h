# test.h

test.h helps you unit testing your C code. It provides simple testing 
functionality in a single header.

## Overview
Currently test.h supports the following assertions
```c
assert(expr)
assert_true(expr)
assert_false(expr)
assert_equal(expr, expected)
assert_not_equal(expr, expected)
assert_null(expr)
assert_not_null(expr)
```

## Example
```c
#include <test.h>

static void
test_example (void)
{
  int a = 21;
  int b = 45;
  int r;

  assert(21 == a);
  assert(45 == b);

  r = a + b;
  assert(r == (a + b));
}

int
main (void)
{
  test_t unit_tests[] = {
    new_test (test_example),
  };
  execute (unit_tests);

  return 0;
}
```