# UMAST

-----
About
-----
UMAST is a tool for calculating maximum agreement subtree for two phylogenetic 
trees.

------------
Installation
------------
Ungzip archive and run makefile.

-----
Input
-----
Two trees in Newick format.

------
Output
------
stdout: by default, the number of leaves in resulting 
agreement subtree; with option `-d`, L-distance between input trees.

umast.tre: file containing maximum agreement subtree in Newick format.

umast.log: text file containing log information about run.

----------------------
Command line arguments
----------------------
`-1` (required) path to file with first input tree

`-2` (required) path to file with second input tree

`-r` option for rooted trees

`-o` path to resulting (u)mast tree file, default ./umast.tre

`-l` path to log file, default ./umast.log

`-d` Print L-distance to stdout instead of agreement subtree size.

`./umast -1 intree1 -2 intree2 [-r] [-o outtree] [-l logfile] [-d]`
