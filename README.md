# test.h

test.h helps you unit testing your C code. It provides simple testing 
functionality in a single header.

## Example

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

Running the example above should produce the following output:

    Tests:
     + test_example                3 assertions passed.

    Summary:
      Type                Ran   Failed
      tests                 1        0
      assertions            3        0

