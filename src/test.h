/**
 *  test.h
 *
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 * 
 * For more information, please refer to <http://unlicense.org/>
 */

#ifndef _TEST_H_
#define _TEST_H_

#include <setjmp.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*test_func_t) (void);

typedef struct
{
  const char *name;
  test_func_t func;
} test_t;

#define new_test(test)  { #test, (test_func_t)test }
#define execute(tests)  _execute(tests, sizeof(tests) / sizeof(test_t))
#define assertion(expr) _assert(__FILE__, __LINE__, #expr, expr)

#define assert(expr)                      assertion(expr)
#define assert_true(expr)                 assertion(expr)
#define assert_false(expr)                assertion(!(expr))
#define assert_equal(expr, expected)      assertion((expr) == (expected))
#define assert_not_equal(expr, expected)  assertion((expr) != (expected))
#define assert_null(expr)                 assert_equal(expr, NULL)
#define assert_not_null(expr)             assert_not_equal(expr, NULL)

#define __try     _try_catch_jmp_buf_valid = true; if (0 == setjmp (_try_catch_jmp_buf))
#define __except  else
#define __done    _try_catch_jmp_buf_valid = false;

static jmp_buf 
_try_catch_jmp_buf;

static bool
_try_catch_jmp_buf_valid = false;

static struct
{
  int assertions_total;
  int tests_failed;
  int tests_total;
} _counter = { 0, 0, 0 };

void
_execute (test_t tests[], size_t test_count)
{
  size_t test_idx;

  _counter.tests_total = 0;
  _counter.tests_failed = 0;
  _counter.assertions_total = 0;

  printf("Tests:\n");

  for (test_idx = 0; test_idx < test_count; test_idx++)
    {
      test_t test = tests[test_idx];

      printf (" + %-20s ", test.name);
      {
        int assertions_initially = _counter.assertions_total;
        __try
          {
            test.func ();
            printf ("%8d assertions passed.\n", _counter.assertions_total - assertions_initially);
          }
        __except
          {
            _counter.tests_failed++;
          }
        __done
      }
      _counter.tests_total++;
    }

  printf("\nSummary:\n");
  printf("  %-14s %8s %8s\n", "Type", "Ran", "Failed");
  printf("  %-14s %8d %8d\n", "tests", _counter.tests_total, _counter.tests_failed);
  printf("  %-14s %8d %8d\n", "assertions", _counter.assertions_total, _counter.tests_failed);
}

void
_assert (char *file, int line, char *statement, bool value)
{
  _counter.assertions_total++;

  if (value)
    return;

  printf ("\nAssertion '%s' in file '%s' on line %d failed.\n", statement, file, line );

  /**
   * If called inside a try-except block jump back to it. Otherwise exit the program. 
   */
  if (_try_catch_jmp_buf_valid)
    longjmp (_try_catch_jmp_buf, 1);
  else
    exit (EXIT_FAILURE);
}

#endif
