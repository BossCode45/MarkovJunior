# This is a basic version of the markov junior algorithm
# Running it
## Pre reqs
+ g++
+ make
## Compiling
run `make`
## Running
run `./main`
## Making the rules
The rules are stored in a variable called `rules` near the top of the code
The heirachy is as follows
```
//Set of groups
{
	//Group 1
	{
		//Equal priority rule group 1
		{
			//Rule 1
			{
				//Sequence to match
				{Red, Green},
				//Sequence to replace it with
				{Red, Blue}, 
				//Number of times the rule can occur (-1 for infinite)
				-1
			},
			//Rule 2
			{{Red, Black}, {Red, Green}, 5},
			etc...
		},
		//Equal priority rule group 2
		{
			//Put rules in here
		},
		etc...
	},
	//Group 2
	{
		//Put rule priority groups and rules in here
	},
	etc...
};
```
+ The rules in a group will be repeated until none of them can be applied
+ The groups of rules will be run sequentially until the last group finishes
+ The board initialises with all blacks
+ You can have 1 or more rules groups, 1 or more priority groups per rule group, 1 or more rules per priority group, 1 or more tiles per sequence
+ You must have the same length sequence to match as sequence to replace
# Credits
+ The markov junior algorithm: Gumin, M. (2022). MarkovJunior, a probabilistic programming language based on pattern matching and constraint propagation (Version 1.0) [Computer software]. https://github.com/mxgmn/MarkovJunior
+ The json parser I'm using: Lohmann, N. (2022). JSON for Modern C++ (Version 3.11.2) [Computer software]. https://github.com/nlohmann
