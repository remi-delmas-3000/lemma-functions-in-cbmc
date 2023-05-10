# lemma-functions-in-cbmc

An experiment expressing lemma functions with CBMC's contract system.

A lemma function is a ghost function equipped with a contract that encodes
a reasoning step where the precondition of the contract is the premise
and the post condition is the lemma of interest.

If proving the lemma requires using inductive reasoning, the body of the lemma
function describes the computation steps encoding the induction schema, either
using a loop with a loop contract, or using a recursive call to the lemma
function. The lemma function must be pure, i.e. not have any side effects on its
arguments.

Proving that the lemma function satisfies its contract amounts to proving that
the lemma is a valid consequence of its premises.

A proved lemma can be used in a different context by calling the lemma function
and abstracting that call by its contract. The contract abstraction checks that
the lemma premises hold and inserts the proved lemma as an assumption.

This allows to reuse the lemma without having to prove it again in context
(this is sound to do since the lemma already been proved in isolation under its
premises).

# Example

The example used here is ported from the Dafny documentation on lemma functions.

The function `find_zero` searches for the first occurrence of zero in an array.

```c
int find_zero(int *a, int a_len)
{
  int i = 0;
  while (i < a_len)
  {
    if (a[i] == 0) {
      return i;
    }
    i = i + a[i];
  }
  return -1;
}
```

It has two preconditions:
- `a[i] >= 0` for all `i` in `[0, a_len)`
- `a[i + 1] >= a[i] - 1` for all `i` in `[0, a_len-1)`

These assumptions allows the search to skip over whole sections of the array at
once while being certain not to miss any zeros, because the next zero is at
least `a[i]` cells away from the current `i`.

To put it more formally, given some array index `i`, if this holds:

```
__CPROVER_forall{ int i; (0 <= i && i < a_len) ==> 0 <= a[i]}) &&
__CPROVER_forall{ int i; (0 <= i && i < a_len-1) ==> a[i + 1] >= a[i] - 1}
```

Then the following holds for any `i` in `[0, < a_len)`:

```
__CPROVER_forall{int j; i <= j && j < i + a[i] ==> a[j] != 0}
```

To prove this we introduce a lemma function `skipping_lemma` where a loop
encodes the induction scheme.

The `skipping_lemma` function is then called in the `find_zero` function to
allow proving the `find_zero` contract's post condition.

# Remarks/Problems

We use loop contracts instrumentation which transforms loops into inductive
base + step formulations.

However we also impose small bounds on array length and small bounds on array
values to prevent pointer and integer overflows.

The proofs use the z3 solver as backend because the SAT backend ignores forall
quantifiers in assumptions.

The `skipping_lemma` analysis succeeds with z3.

The `find_zero` analysis fails with a z3 error (quantified expression in a get-value command).

The `find_zero_manual` model contains a manual loop contract transformation of
find zero and manually written abstraction stub for `skipping_lemma`.

It proves successfully with z3 buth fails with the SAT backend (ignored quantifiers).

The runtime and memory consumption of the SMT solver augment with the array size,
since we use induction solving should not be sensitive to the array size.
