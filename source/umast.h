/*  Copyright 2016, 2017 Andrey Sigorskikh, Sergei Spirin

    This file is part of UMAST.

    UMAST is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    UMAST is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with UMAST (a file named "COPYING.txt").
    If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _UMAST_H_
#define _UMAST_H_
#include "Tree.h"
#include "add.h"
#include "Branch.h"
#include "BranchArray.h"
#include "BranchAllocator.h"
#include <stdlib.h>
#include <string.h>

const int nodeTypeCount = 6;

BranchArray* treeRootedToBranchArray(Tree* tree, int* permutation);
Tree* deleteLeaves(Tree* tree, char** leavesToDelete, int leavesToDeleteAmount);
Tree** treesPrune(Tree* tree1, Tree* tree2);
Tree* treeRoot(Tree* tree, unsigned nodeID, unsigned neighbourID, char newTree);
Tree* treeUnRoot(Tree* tree, char newTree);
unsigned* treeRootAndTopSort(Tree* tree, unsigned nodeID, unsigned neighbourID, unsigned* setPermutation, int unrooted);
int* calculateLeavesPermutation(Tree* tree1, Tree* tree2);
int branchGetLeavesPosNum(Branch* br);
int* getTreeLeavesPos(Tree* tree);
unsigned* branchToLeavesArr(Branch* br, unsigned leavesNum);
int** getAllRoots(Tree* tree);
int* getRandMaxBranch(Branch*** TAB, int rows, int cols);
Tree* makeMAST(Branch* br, Tree* tree1);
void countVariants(Branch*** TAB, int a, int w, int b, int c, int x, int y,
    int* bestValue, int* bestPos);
Branch* MAST(Tree* tree1, Tree* tree2, unsigned* set1, unsigned* set2, unsigned* setPermutation1, unsigned* setPermutation2);
Branch* UMASTStep(Tree* intree1, Tree* intree2, unsigned* set1, unsigned* set2,
    unsigned* setPermutation1, unsigned* setPermutation2, Branch**** TAB, Branch** rootRow,
    BranchAllocator* brAllocator);
Tree* makeUMASTTree(Branch* br, Tree* tree1);
void UMAST(Tree* tree1, Tree* tree2);
void RMAST(Tree* tree1, Tree* tree2);
void printUmastHelp(char* name);
#endif
