# Definitions:

## Input:
>m number of columns

>n number of rows

>a vector of integers per each row and column that where each integer represents the number of concurrent marked blocks in that same order

## Output:
>m*n matrix of bool elements stating which elements are marked  and not


# Analysis:

## Problems:
When researchers describe nonograms as being NP-Complete, they describe the Solvability or Existence problem. Citing R.A. Oosterman,et.al 2007, the problems surrounding nonograms are three:

>1. Existence: given a Nonogram N, does there exist a solution. In other words, is >this Nonogram solvable?
>2. Solving: given a solvable Nonogram N, how can we obtain a solution?
>3. Uniqueness: given a solution for a Nonogram N, does there exist another solution?

## Scope:

The scope of this work/project is to solve the SECOND problem, the SOLVING problem as defined above. In other words, obtaining a solution given a solvable Nonogram N.

I ensure the nonogram is solvable, by first producing a complete nonogram and then obtaining its input and not using/deleting the solution nonogram, reconstructing the solution from exclusively the input (see input above). 

The first and second problem could have overlap in its solving methods, and a fully accurate solution to the second problem could potentially solve the first problem if it simply does not find a solution.

## Motivation:
so brute forcing this would take 
>O(2^(n*m))
very unefficient.

the goal is to optimize the solving sufficiently so that it takes a reasonable amount of time to solve a 1000x1000 puzzle and to see the solving process.

Thus, the main concern is optimization.

## Approaches:
## Logic-Only:
A heuristic process where all human-like solving methods are done obtaining the most amount of data the input gives us before any guessing is needed for 

## DFS:
### Determining all possibilities of a row:
The approach taken by Oosterman,et.al 2007 (once again) is:
>Find all possible combinations for a row or column. 
>Then filter out all of the candidate solutions that do not adhere to the description, based on the pixels that already have been given a known value.

This is done by using the permutationVector Custom class particularly specialized for this purpose. More on it in [permutationVector.h](./src/permutationVector.h). 
we do not store all possible combinations but rather iterate through permutation vector (treating it like a counting problem) and check this possibility to whether it adheres to cells (or pixels) already given a value

As you might suspect, this step by itself is NP-Complete.

### Inorder-tree DFS traversal:
recursively traverse all possibilities treating the nonogram's possible rows as a node of a tree, with higher rows being parents of lower rows
Reaching, eventually a solution.

### Considerations:
Memory was kept limited, in order to make DFS work and have it be possibly aided by the Logic-Only method above as a precursor,
another nonogram bool matrix was added, thus making the nonogram object in itself heavier in memory even if only using the Logic-Only methods

a compromise on memory could be made, storing exponentially more memory in each step to make this method more efficient, currently avoided.

future plans are to integrate logic solving into DFS (first method above), to decrease the amount of possibilities that need to be checked by contradictions arising
