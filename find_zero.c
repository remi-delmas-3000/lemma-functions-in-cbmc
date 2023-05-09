#include <stdbool.h>
#include <stdlib.h>
#include "skipping_lemma.h"

int find_zero(int *a, int a_len)
    // clang-format off
__CPROVER_requires(0 < a_len && a_len <= 100)
__CPROVER_requires(__CPROVER_is_fresh(a, sizeof(int) * a_len))
__CPROVER_requires(__CPROVER_forall{ int i; (0 <= i && i < a_len) ==> (0 <= a[i] && a[i] <= 100)})
__CPROVER_requires(__CPROVER_forall{ int i; (0 < i && i < a_len) ==> a[i-1]-1 <= a[i]})
// __CPROVER_ensures((__CPROVER_return_value < 0) ==> __CPROVER_forall{ int i; 0 <= i && i< a_len ==> a[i] != 0})
__CPROVER_ensures((0 <= __CPROVER_return_value) ==> (__CPROVER_return_value < a_len) && (a[__CPROVER_return_value] == 0))
// clang-format on
{
  int index = 0;
  while (index < a_len)
    // clang-format off
  __CPROVER_assigns(index)
  __CPROVER_loop_invariant(0 <= index && index <= a_len)
  __CPROVER_loop_invariant(__CPROVER_forall{ int k; (0 <= k && k < a_len) ==> (k < index ==> (a[k] != 0))})
  __CPROVER_decreases(100-index)
  // clang-format on
  {
    assert(0 <= a[index] && a[index] <= 100);
    if (a[index] == 0) {
      return index;
    }
    skipping_lemma(a, a_len, index);
    index = index + a[index];
    }
  return -1;
}

int main() {
  int *a;
  int a_len = 10;
  find_zero(a, a_len);
}