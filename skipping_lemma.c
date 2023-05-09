#include <stdbool.h>
#include <stdlib.h>
#include "skipping_lemma.h"

void skipping_lemma(int *a, int a_len, int j)
{
  int i = j;
  while ((i < a_len) && (i < j + a[j]))
    // clang-format off
    __CPROVER_assigns(i)
    __CPROVER_loop_invariant(0 <= i && i <= a_len)
    __CPROVER_loop_invariant((i < a_len) ==> (a[j] - (i-j) <= a[i]))
    __CPROVER_loop_invariant(__CPROVER_forall{int k; (0 <= k && k <= a_len) ==> ((j <= k && k < i) ==> a[k] != 0)})
    __CPROVER_decreases(-i)
    // clang-format on
    {
      i = i + 1;
    }
}

int main() {
  int *a;
  int a_len;
  int j;
  skipping_lemma(a, a_len, j);
  return 0;
}
