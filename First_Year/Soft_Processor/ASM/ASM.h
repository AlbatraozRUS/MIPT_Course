
#ifndef ASM_ASM_H
#define ASM_ASM_H

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <cctype>
#include <string>


const char Input_File[]  = "/Users/albatraozrus/Desktop/Square.txt";
const char Output_File[] = "/Users/albatraozrus/Desktop/Output_File.txt";
const char ErrorWhichCommand = 255;
const int Poison = -666;
const int AmountOfLabels = 30;

struct Line {
    char *ptrbuffer;
    unsigned long length;
};

struct Marks {
  int Address = 0;
  char *NameLabel = nullptr;
};

int Assembler (char *bytecode, size_t *amount);
struct Line * Reading(size_t *amount);
size_t CounterOfLines(char *buffer, long size);
void SurgeryOfLines(char *buffer, long size);
long CounterOfSymbols(FILE * readfile);
struct Line* CreateStruct(char *buffer, size_t size, size_t amount);
char WhichCommand (char *Command);
void DeleteComments (struct Line *ptrstruct, size_t amount);
void Writing (char *bytecode, int size);
bool PassTrash(char *Command);
bool ScanCommand (char *Command, struct Line *ptrstruct, int NumberOfLine);
int WhichSpec (struct Line *ptrstruct, int NumberOfLine);
void TransformName(struct Line *ptrstruct, int NumberOfLine, struct Marks *Lables,
                  int CountLables, char *bytecode, int *PC);
bool LocateLabels(struct Line *ptrstruct, int NumberOfLine, struct Marks *Labels, int *CountLabels, int *PC);
void ScanParam (int Spec, struct Line *ptrstruct, int NumberOfLine, unsigned int *Param, int *PC, char *bytecode);
void FullBytecode (int *PC, char *bytecode, char *Command_Number, char *Command,
                   int NumberOfLine, struct Line *ptrstruct, unsigned int *Param, struct Marks *Lables, int CountLables);
void Compilation (char *Command_Number, int *PC, int *CountLabels, struct Marks *Labels,
                  struct Line *ptrstruct, char *Command, char *bytecode, unsigned int *Param);
void DumpASM (char *Command_Number, int *PC, struct Line *ptrstruct, char *Command, char *bytecode, int NumberOfLine);

#endif //ASM_ASM_H
