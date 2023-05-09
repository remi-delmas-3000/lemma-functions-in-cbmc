skipping_lemma:
	goto-cc skipping_lemma.c
	goto-instrument --dfcc main --enforce-contract skipping_lemma --apply-loop-contracts a.out b.out
	cbmc --z3 b.out

find_zero:
	goto-cc find_zero.c
	goto-instrument --dfcc main --enforce-contract find_zero --replace-call-with-contract skipping_lemma --apply-loop-contracts a.out b.out
	cbmc --stop-on-fail --z3 b.out

find_zero_manual:
	cbmc --external-sat-solver kissat find_zero_manual.c

find_zero_manual_smt2:
	cbmc --stop-on-fail --z3 find_zero_manual.c
