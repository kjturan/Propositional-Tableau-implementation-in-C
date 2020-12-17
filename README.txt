This program will read its input from a file called input.txt, parse each of the ten formulas and output their types, build a tableau for each of the
ten formulas, expand till completed, test to see if they are closed, and then output whether the
formula is satisfiable or not. All the outputs go to a file called output.txt.

To run this program  run the following commands in the respective directory:

$ gcc skeleton.c
$ ./a.out

To test the parser on your own examples, simply add them to input.txt with the following notation:

[Original Symbol] : [Notation]
------------------------------
AND : ^
OR  : v
NEGATION : -
IMPLICATION : > 

Note: The parser currently only works with propositional constants p, q, and r.
Further work will allow the parser to work with an arbitrary number of constants. 
