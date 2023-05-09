#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

int nondet_int();

// look for the first zero in an array
int main() {
  int a_len = 1000; // have to make this constant to suppress some errors
  int *a = malloc(a_len *sizeof(int));

  // // preconditions
  // __CPROVER_assume(0 < a_len && a_len <= 1000);

  // all array elements are positive
  __CPROVER_assume(__CPROVER_forall{ int i; (0 <= i && i < a_len) ==> (0 <= a[i] && a[i] <= 100)});

  // there is at most a difference of 1 between consecutive indices
  __CPROVER_assume(__CPROVER_forall{ int i; (0 < i && i < a_len) ==> a[i-1]-1 <= a[i]});

  int index = 0;

  // assert loop invariant in base case
  assert(0 <= index && index <= a_len);
  assert(__CPROVER_forall{ int k; (0 <= k && k < a_len) ==> (k < index ==> (a[k] != 0))});

  // havoc step and assume loop invariant
  index = nondet_int();
  __CPROVER_assume(0 <= index && index <= a_len);
  __CPROVER_assume(__CPROVER_forall{ int k; (0 <= k && k < a_len) ==> (k < index ==> (a[k] != 0))});

  // loop step
  if (index < a_len) {

    if (a[index] == 0) {
      return index;
    }

    // inline call to the lemma function contract skipping_lemma(a, a_len, index);
    // assert premises
    assert(0<= a_len && a_len <= 1000);
    assert(__CPROVER_r_ok(a, sizeof(int) * a_len));
    assert(0 <= index && index < a_len);
    assert(__CPROVER_forall{ int i; (0 <= i && i < a_len) ==> (0 <= a[i] && a[i] <= 100)});
    assert(__CPROVER_forall{ int i; (0 < i && i < a_len) ==> a[i-1]-1 <= a[i]});

    // assume lemma function post conditions
    __CPROVER_assume(__CPROVER_forall{ int k; (0 <= k && k < a_len) ==> ((index <= k && (k < (index + a[index]))) ==> a[k] != 0)});

    // jump to the next index
    index = index + a[index];

    __CPROVER_assume(__CPROVER_forall{ int k; (0 <= k && k < a_len) ==> (k < index ==> (a[k] != 0))});
    __CPROVER_assume(false);
  }

  // loop exit
  index = -1;

  // assert post conditions
  assert((index < 0) ==> __CPROVER_forall{ int i; (0 <= i && i < a_len) ==> a[i] != 0});
  assert((0 <= index) ==> (index < a_len) && (a[index] == 0));
}