#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "constants.h"

int nondet_int();


// contract-derived abstraction for skipping_lemma
void skipping_lemma(int *a, int a_len, int j) {
  // assert preconditions
  assert(0 < a_len && a_len <= MAX_A_LEN);
  assert(__CPROVER_r_ok(a, sizeof(int) * a_len));
  assert(0 <= j && j < a_len);
  assert(__CPROVER_forall {
    int i;
    (0 <= i && i < a_len) ==> (0 <= a[i] && a[i] <= MAX_A_VAL)
  });
  assert(__CPROVER_forall {
    int i;
    (0 < i && i < a_len) ==> a[i - 1] - 1 <= a[i]
  });

  // assume lemma function post conditions
  __CPROVER_assume(__CPROVER_forall {
    int k;
    (0 <= k && k < a_len) ==> ((j <= k && (k < (j + a[j]))) ==> a[k] != 0)
  });
}

// verification wrapper for __find_zero
int find_zero(int *a, int a_len) {
  // allocate parameters/assume preconditions
  int a_len = nondet_int();
  __CPROVER_assume(0 < a_len && a_len <= MAX_A_LEN);

  int *a = malloc(a_len *sizeof(int));
  __CPROVER_asume(a);

  __CPROVER_assume(__CPROVER_forall {
    int i;
    (0 <= i && i < a_len) ==> (0 <= a[i] && a[i] <= MAX_A_VAL)
  });

  __CPROVER_assume(__CPROVER_forall {
    int i;
    (0 < i && i < a_len) ==> a[i - 1] - 1 <= a[i]
  });

  // call function
  int index = __find_zero(a, a_len);

  // assert post conditions
  assert((index < 0) ==> __CPROVER_forall {
    int i;
    (0 <= i && i < a_len) ==> a[i] != 0
  });

  assert((0 <= index) ==> (index < a_len) && (a[index] == 0));
  return index;
}


// original function with loop contract transformation manually applied
int __find_zero(int *a, int a_len) {

  int index = 0;

  // assert loop invariant in base case
  assert(0 <= index && index <= a_len);
  assert(__CPROVER_forall{
    int k;
    (0 <= k && k < a_len) ==> (k < index ==> (a[k] != 0))
  });

  // step case : havoc loop state and assume loop invariant
  index = nondet_int();
  __CPROVER_assume(0 <= index && index <= a_len);
  __CPROVER_assume(__CPROVER_forall{
    int k;
    (0 <= k && k < a_len) ==> (k < index ==> (a[k] != 0))
  });

  // loop step
  if (index < a_len) {

    if (a[index] == 0) {
      return index;
    }
    skipping_lemma(a, a_len, index);
    index = index + a[index];

    // prove loop invariant
    __CPROVER_assume(__CPROVER_forall{
      int k;
      (0 <= k && k < a_len) ==> (k < index ==> (a[k] != 0))
    });
    __CPROVER_assume(false);
  }
  return -1;
}

int main() {
  int *a;
  int a_len;
  find_zero(a, a_len);
  return 0;
}