
#ifndef HASH_TABLE_HASH_TABLE_H
#define HASH_TABLE_HASH_TABLE_H

#include <cstdlib>
#include <cstdio>
#include "List.h"

const unsigned int SizeOfTable = 1000;
const char Input_File[] = "/home/albatraozrus/Desktop/Text.txt";

struct Line {
    char *ptrbuffer;
    unsigned long length;
};

#define FILLHASHTABLE(Table, Strings, amount, FunctionName,File){\
    for (int Line = 0; Line < amount; Line++)\
        PushBegin(&Table[FunctionName % SizeOfTable], Strings[Line].ptrbuffer);\
    Writting(Table,File);\
    }

struct MyList* InitHashTable();
void HashDUMP(MyList *Table);
bool FillHashTable(MyList *Table);
bool Writting(MyList *Table, FILE *Results);
struct Line * Reading(size_t *amount);
bool DecorateFile(FILE *Results);
bool SearchByValue(MyList *List, char *Value, int Size);




unsigned int NotReallyHash();
unsigned int FirstLetter(const char *String);
unsigned int Length(const char *String);
unsigned int SumOfLetters (const char *String, int Length);
int Roll(const char* String);
int GNUHash(const char* string, char stopper);
unsigned int MurmurHash (char * key, unsigned int len);
void Clear(MyList *Table);


#endif //HASH_TABLE_HASH_TABLE_H
