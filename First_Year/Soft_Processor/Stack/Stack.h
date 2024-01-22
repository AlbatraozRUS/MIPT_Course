#ifndef STACK_STACK_H
#define STACK_STACK_H

#include <cstdio>
#include <memory>
#include <cstdlib>

typedef int Elem_t;
#define Elem_TSpec "%d"

typedef int Litmus_t;

const Litmus_t LitmusBeginValue = 1234;
const Litmus_t LitmusEndValue   = 9876;

const Elem_t Poison = 111222;

#define CalcHashSum stk->HashSum = MurmurHash((char*) stk->value, sizeof(Elem_t) * stk->size);
#define MEOWMEOW

#ifdef MEOWMEOW
#define assertMyStack(PointerToStack) {\
    if ( StackOK(PointerToStack) == false)  {\
         printf(" There is an Error occured in the stack\n");\
         printf("Error occurred in function - %s\n",  __PRETTY_FUNCTION__);\
         printf("on the line: %d\n", __LINE__);\
         printf("Please check the Dump\n");\
         Dump(PointerToStack);\
         abort();\
    }\
}

#else
#define assertMyStack(PointerToStack)
#endif

struct MyStack_t
{
    Litmus_t LitmusBegin     = Poison;
    Litmus_t *ptrLitmusBegin = nullptr;

    char *data     = nullptr;
    size_t size    = Poison;
    size_t MaxSize = Poison;
    Elem_t HashSum = Poison;
    Elem_t *value  = nullptr;
    int ErrorCode  = Poison;

    Litmus_t * ptrLitmusEnd = nullptr;
    Litmus_t LitmusEnd      = Poison;
};

enum TypesOfErrors{
    EUNDERFLOW       = 111,
    EADDMEM          = 112,
    DATANULLPTR      = 211,
    SIZELESS0        = 311,
    MAXSIZELESS0     = 312,
    SIZEMOREMAXSIZE  = 313,
    ELITMUSBEGIN     = 411,
    ELITMUSEND       = 422,
    EHASHSUM         = 511

};

bool StackConstruct (MyStack_t *stk);
bool StackFirstInit (MyStack_t *stk);
bool StackPush(MyStack_t *stk, Elem_t elem);
Elem_t StackPop(MyStack_t *stk);
bool StackDestruct(MyStack_t *stk);
char * StackAddMem (MyStack_t *stk, size_t size);
char* StackDecMem (MyStack_t *stk, size_t size);
bool StackDataLitmus (MyStack_t *stk);
bool StackOK(MyStack_t *stk);
void Dump(MyStack_t *stk);
void UnitTests(MyStack_t *stk);
unsigned int MurmurHash (char * key, unsigned int len);


#endif //STACK_STACK_H
