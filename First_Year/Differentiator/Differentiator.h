#ifndef DIFFERENTIATOR_DIFFERENTIATOR_H
#define DIFFERENTIATOR_DIFFERENTIATOR_H

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <string>

const unsigned POISON = 66066;

const char DOT[]        = "/Users/albatraozrus/Desktop/GraphwyzDiff.dot";
const char Input_File[] = "/Users/albatraozrus/CLionProjects/Differentiator/Input.txt";
const char TEX_Output[] =  "/Users/albatraozrus/Desktop/Latex_Diff.tex";

const unsigned SizeOfString = 10;

struct Data {
  unsigned Type  = POISON;
  int ElemData   = POISON;
  char *Function = nullptr;
};

struct Branch {
  Branch *Parent = nullptr;
  Branch *Left   = nullptr;
  Branch *Right  = nullptr;
  Data   *Elem   = nullptr;
};

struct Tree {
  unsigned size = POISON;
  Branch *Head = nullptr;
};


struct Line {
    char *ptrbuffer;
    unsigned long length;
};

enum Types {
  MODE_OPERATOR = 1,
  MODE_VARIABLE = 11,
  MODE_NUMBER   = 111,
  MODE_FUNCTION = 1111,
  NIL           = '$',
};

#define ADDDIFF(Left, Right) {NewBranch(MODE_OPERATOR, nullptr, '+', Diff(Left), Diff(Right))}
#define SUBDIFF(Left, Right) {NewBranch(MODE_OPERATOR, nullptr, '-', Diff(Left), Diff(Right))}
#define MULDIFF(Left, Right) {NewBranch(MODE_OPERATOR, nullptr, '+', NewBranch(MODE_OPERATOR, nullptr, '*',\
                    Diff(Left), Copy(Right)),  NewBranch(MODE_OPERATOR, nullptr, '*',\
                           Copy(Left), Diff(Right)))}
#define DIVDIFF(Left, Right) {NewBranch(MODE_OPERATOR, nullptr, '/', NewBranch(MODE_OPERATOR, nullptr, '-', \
                    NewBranch(MODE_OPERATOR, nullptr, '*', Diff(Left), Copy(Right)),\
                    NewBranch(MODE_OPERATOR, nullptr, '*', Copy(Left), Diff(Right))),\
                     NewBranch(MODE_OPERATOR,nullptr, '^', Copy(Right),\
                      NewBranch(MODE_NUMBER, nullptr, 2, nullptr, nullptr)))}
#define PAWDIFF(Left, Right) {NewBranch(MODE_OPERATOR, nullptr, '*', Copy(Right), \
                            NewBranch(MODE_OPERATOR, nullptr, '^', Copy(Left),\
                            NewBranch(MODE_OPERATOR, nullptr, '-', Copy(Right), \
                            NewBranch(MODE_NUMBER, nullptr, 1 , nullptr, nullptr))))}

struct Line * Reading();
Branch *CreateBranch (Branch *Parent);
bool FillTree (Branch *Node, Line *Text, unsigned int *Letter);
bool FillBranch (Branch *Node, unsigned Type, char *String, int Number);
void Analyze (Line *Text, unsigned *Letter, Branch *Node, int *Number,
        char *String, unsigned *ScannedLetters);
Branch *Diff (Branch *Node);
Branch *NewBranch (unsigned Type, char *String, int Number, Branch *LeftNode, Branch *RightNode);
void MakeEasier (Branch *Node);
bool MakeSimple (Branch *Node);
Branch *DiffFunction (Branch *Node);
void Latex (Branch *Node, FILE *Output);
Branch *Copy (Branch *Node);
void PrintLatex (Branch *Node, FILE *Output);
void NextEquation (FILE *Output, Branch *Node);
bool CheckForLeafs(Branch *Node);
Branch* GetG (const char *Expression);
void ChangeChilds (Branch *Node, int Child);
void Sentences (FILE *Output);
bool DestBranch (Branch *Node);



void tree_dot(Branch* nameTree);

#endif //DIFFERENTIATOR_DIFFERENTIATOR_H
