
#ifndef AKINATOR_TREE_H
#define AKINATOR_TREE_H

#include <cstdlib>
#include <cassert>
#include <cstring>

struct Branch{
  char *data     = nullptr;
  Branch *parent = nullptr;
  Branch *left   = nullptr;
  Branch *right  = nullptr;
  int Number     = 1;
};

struct Tree {
  Branch *Head = nullptr;
  unsigned int size= 0;
};

Branch *CreateBranch (Branch *Parent, char *data, unsigned int *Size);
//bool DestBranch (Branch *Node);

#endif //AKINATOR_TREE_H
