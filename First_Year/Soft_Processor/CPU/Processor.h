#ifndef PROCESSOR_PROCESSOR_H
#define PROCESSOR_PROCESSOR_H

#include <cassert>
#include <cmath>
#include "Stack.h"

const char Input_File[] = "/Users/albatraozrus/Desktop/Output_File.txt";


#define PC_MOVE(Mode) {\
if ((Mode) == 'i')\
    PC += 2 * sizeof(char) + sizeof(int);\
if ((Mode) == 'c')\
    PC += 3 * sizeof(char);\
if ((Mode) == 'ij')\
    PC += sizeof(char) + sizeof(int);\
}

struct MYCPU {
    MyStack_t stk;
    MyStack_t Addresses;

    int CR[4] = {};
};

enum Specs{
    NUMBER   = 1,
    REGISTER = 2,
    NOTHING  = 3,
};

void CPU_Reader (char *bytecode, MYCPU *CPU);
char * Reading(size_t *amount);
size_t CounterOfLines(char *buffer, long size);
size_t CounterOfSymbols(FILE * readfile);
void DumpCPU (const MYCPU* CPU, const char* cmdName, int cmdNum, int PC, const char* bytecode);

#endif //PROCESSOR_PROCESSOR_H
