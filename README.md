# Propositional-Tableau-implementation-in-C
A parser, written in C, that determines the satisfiability of a propositional formula. 

This program will read its input from a file called input.txt and, for each formulas present, 
do the following:

- Parse the input
- Build a propositional tableau (https://en.wikipedia.org/wiki/Method_of_analytic_tableaux)
- Expand till completed
- Test to see if it closed
- And output the formula type and whether it is satisfiable or not. 

All the outputs are collected and outputted to file called output.txt.

To run this program, run the following commands in the respective directory:

'''
$ gcc skeleton.c
$ ./a.out
'''

To test the parser on your own examples, simply add them to input.txt with the following notation:

'''
[Original Symbol] : [Notation]
AND : ^
OR  : v
NEGATION : -
IMPLICATION : > 
'''

Note: The parser currently only works with propositional constants p, q, and r.
Further work will allow the parser to work with an arbitrary number of constants. 
