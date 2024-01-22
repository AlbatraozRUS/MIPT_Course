
#ifndef AKINATOR_AKINATOR_H
#define AKINATOR_AKINATOR_H

#include <cstdio>
#include <cassert>
#include "Tree.h"

const char DOT[] = "/Users/albatraozrus/Desktop/GraphwyzAkin.dot";
const char Input_File[] = "/Users/albatraozrus/CLionProjects/Akinator/Input.txt";
const size_t SizeOfString = 50;


struct Line {
    char *ptrbuffer;
    unsigned long length;
};


struct Line * Reading();
bool FillTree (Branch *Node, Line *Text, unsigned int *Letter, unsigned int *Size);
void DotForAkinator (FILE *dot_out, Branch *Head);
bool ExploreTree (Branch *Node, unsigned int *Size);
bool Result(Branch *Node, unsigned int *Answer, unsigned int *Size);
bool AddNewAnswer (Branch *Node, unsigned int *Size);
bool Writting (Branch *Node, FILE *Output_File);
bool WayToElem (Branch *Node, char *String);
char *Dialogue (char *Question);
bool PrintWay(Branch *Node, char *String);
bool Compare (Branch *Node, char *FirstElement, char *SecondElement, unsigned int Size);
void Menu (Branch *Root, unsigned *Size);

#endif //AKINATOR_AKINATOR_H
