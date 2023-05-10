# works
skipping_lemma:
	goto-cc skipping_lemma.c
	goto-instrument --dfcc main --enforce-contract skipping_lemma --apply-loop-contracts a.out b.out
	cbmc --bounds-check --pointer-check --signed-overflow-check --unsigned-overflow-check --z3 b.out

# does not work
find_zero:
	goto-cc find_zero.c
	goto-instrument --dfcc main --enforce-contract find_zero --replace-call-with-contract skipping_lemma --apply-loop-contracts a.out b.out
	cbmc --bounds-check --pointer-check --signed-overflow-check --unsigned-overflow-check --stop-on-fail --z3 b.out

# does not work
find_zero_manual:
	cbmc --external-sat-solver kissat find_zero_manual.c

# works
find_zero_manual_smt2:
	cbmc --bounds-check --pointer-check --signed-overflow-check --unsigned-overflow-check --stop-on-fail --z3 find_zero_manual.c
