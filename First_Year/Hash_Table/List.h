
#ifndef LIST_LIST_H
#define LIST_LIST_H

#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

typedef char* Elem_t;

const int POISON = -666;
const int InitSize = 10;
const int ElementNotFound = -999;

enum ErrorCodes{
  NOERRORS = -1,
  EFIRSTELEM = 1,
  ETALE = 100,
  EHEAD = 11,
  EADDMEM = 911,
};

struct ListElem {
   Elem_t data = nullptr;
   int next    = POISON;
   int prev    = POISON;
};

struct MyList {
   ListElem *Element = nullptr;
   int head          = POISON;
   int tale          = POISON;
   int ErrorCode     = POISON;
   int free          = 0;
   size_t size       = (size_t) POISON;
   size_t MaxSize    = 0;
};

#define MEOWMEOW0

#ifdef MEOWMEOW
#define AssertList(PointerToList) {\
    if (ListOK(PointerToList) == false)  {\
         printf(" There is an Error occured in the List\n");\
         printf("Error occurred in function - %s\n",  __PRETTY_FUNCTION__);\
         printf("on the line: %d\n", __LINE__);\
         printf("Please check the Dump\n");\
         DUMP(PointerToList);\
         abort();\
    }\
}

#else
#define AssertList(PointerToList)
#endif



#define Check(Param)\
{if (List->Element[List->Element[Param].next].data == nullptr)\
    {List->tale = Param;\
    List->Element[Param].next = 0;}\
if (List->Element[List->Element[Param].prev].data == nullptr)\
    {List->head = Param;\
    List->Element[Param].prev = 0;}\
if (List->Element[List->Element[Param].next].prev == -1)\
    List->tale = Param;\
if (List->Element[List->Element[Param].prev].prev == -1)\
    List->head = Param;}

bool CreateList (MyList* List);
bool InitList (MyList *List);
bool InsertBefore (MyList *List, int index, Elem_t Elem);
bool InsertAfter (MyList *List, int index, Elem_t Elem);
bool PushBegin(MyList *List, Elem_t Elem);
bool PushEnd(MyList *List, Elem_t Elem);
bool DestructList (MyList *List);
int SearchElem (MyList* List, Elem_t Elem);
void DUMP (MyList* List);
bool Delete (MyList *List, int index);
bool ListOK(MyList *List);
bool AddMemList (MyList *List);
bool ClearList(MyList *List);

#endif //LIST_LIST_H
