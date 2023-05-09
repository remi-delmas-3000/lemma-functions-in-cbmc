#include <stdbool.h>
#include <stdlib.h>

// clang-format off
void skipping_lemma(int *a, int a_len, int j)
// stay within conservative limits of the pointer model
__CPROVER_requires(0 <= a_len && a_len <= 100)
__CPROVER_requires(__CPROVER_is_fresh(a, sizeof(int) * a_len))
__CPROVER_requires(0 <= j && j < a_len)
// all values positive
__CPROVER_requires(__CPROVER_forall{ int i; (0 <= i && i < a_len) ==> (0 <= a[i] && a[i] <= 100)})
// difference of 1 at most between consecutive values
__CPROVER_requires(__CPROVER_forall{ int i; (0 < i && i < a_len) ==> a[i-1]-1 <= a[i]})
// we only skip non zero values by skipping from the value at once cell
__CPROVER_ensures(__CPROVER_forall{ int k;  (0 <= k && k < a_len) ==> (j <= k && (k < (j + a[j])) ==> a[k] != 0)})
;
// clang-format on
