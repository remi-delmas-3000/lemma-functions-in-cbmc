#ifndef SKIPPING_LEMMA_H_DEFINED
#define SKIPPING_LEMMA_H_DEFINED

#include "constants.h"
#include <stdbool.h>
#include <stdlib.h>

// clang-format off
void skipping_lemma(int *a, int a_len, int j)
__CPROVER_requires(0 < a_len && a_len <= MAX_A_LEN) // stay within limits of the pointer model
__CPROVER_requires(__CPROVER_is_fresh(a, sizeof(int) * a_len))
__CPROVER_requires(0 <= j && j < a_len)
// all values positive and small
__CPROVER_requires(__CPROVER_forall{ int i; (0 <= i && i < a_len) ==> (0 <= a[i] && a[i] <= MAX_A_VAL)})
// difference of 1 at most between consecutive values
__CPROVER_requires(__CPROVER_forall{ int i; (0 < i && i < a_len) ==> a[i-1]-1 <= a[i]})
// we only skip non zero values by skipping from the value at once cell
__CPROVER_ensures(__CPROVER_forall{ int k;  (0 <= k && k < a_len) ==> (j <= k && (k < (j + a[j])) ==> a[k] != 0)})
;
// clang-format on
#endif