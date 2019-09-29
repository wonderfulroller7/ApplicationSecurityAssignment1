# ApplicationSecurityAssignment1
To build a spell checker
-------------------------

[![Build Status](https://travis-ci.com/wonderfulroller7/ApplicationSecurityAssignment1.svg?token=hJq5j26qqMLZspbxj2hm&branch=master)](https://travis-ci.com/wonderfulroller7/ApplicationSecurityAssignment1)

- Part 1:
	- Create GPG key to make sure that all our commits can be verified.
	- Make sure that Travis correctly builds the program
	- Update Configuration files for Travis
	- Procedure to commit for Signature to be verified
		- git commit -S -m '<message to be added>' #(note -S is what signs, -s will not sign)

- Part 2:
	- Program spell-check
	- Analyze the output using Valgrind
	- Program should run as **./spell_check <wordfile> <dictionary_file>**.
	- Code edits mainly occur in spell.c

- Part 3:
	- Create testcases
	- Fix memory leaks stated by Valgrind
	- Run the AFL fuzzer
