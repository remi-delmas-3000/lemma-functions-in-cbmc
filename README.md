# lemma-functions-in-cbmc

An experiment expressing lemma functions with CBMC's contract system.

A lemma function is a ghost function equipped with a contract
where the precondition is the premise of the lemma and the post condition is
the lemma of interest.

If proving the lemma requires to inductive reasoning, the body of the lemma
function describes the computation steps encoding the induction schema
(using a loop with a contract, or a recursive call to the lemma function).

Proving that the lemma function satisfies its contract amounts proves that the
lemma is a valid consequence of its premises.

A ghost call to the lemma function can then be made in the context of another
analysis, and replaced by its contract. This amounts to proving that the
premises hold and injecting the valid lemma as an assumption in the local
context, thereby encapsulating the inductive reasoning that initially took place
to prove the lemma.

All the examples here use inductive base + step formulations for loops, however
we impose small bounds on array length and array values to prevent pointer and
integer overflow problems. This also makes the model provable using CBMC's eager
grounding of quantifiers.
