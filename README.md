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
stdout: distance between trees calculated as number of leaves in resulting 
agreement subtree or L-distance between input trees.

umast.tre: file containing maximum agreement subtree in Newick format.

umast.log: text file containing log information about run.

----------------------
Command line arguments
----------------------

-r option for rooted trees

-o path to resulting (u)mast tree file, default ./umast.tre

-l path to log file, default ./umast.log

-d Print L-distance to stdout instead of agreement subtree size.

./umast -1 tree1_path -2 tree2_path [-r] [-o outtreefile] [-l logfile] [-d]
