#include "List.h"


bool CreateList (MyList* List)
{
    List->head      = 0;
    List->tale      = 0;
    List->free      = 0;
    List->size      = 0;
    List->MaxSize   = 0;
    List->ErrorCode = NOERRORS;

    return true;
}

bool InitList (MyList *List)
{

    List->MaxSize = InitSize;
    List->Element = (ListElem*) calloc(List->MaxSize, sizeof(ListElem));
    for (int index = 0; index< InitSize; index++)
        List->Element[index].data = nullptr;

    List->head      = 0;
    List->tale      = 0;
    List->free      = 1;
    List->ErrorCode = NOERRORS;

    for (int IndexForFree = 1; IndexForFree < InitSize - 1; IndexForFree++)
        List->Element[IndexForFree].next = IndexForFree + 1;
    List->Element[List->MaxSize - 1].next = 0;
    for (int IndexForFree = 2; IndexForFree < InitSize; IndexForFree++)
        List->Element[IndexForFree].prev = -1;

    return true;
}

int SearchElem (MyList* List, Elem_t Elem)
{
    for (int index = 1; index < List->MaxSize; index++){
       // printf("%s\n", List->Element[index].data);
        if (strcmp(List->Element[index].data, Elem) == 0)
            return index;
    }
    return ElementNotFound;
}

bool PushBegin(MyList *List, Elem_t Elem)
{
   AssertList(List);

   int NewIndex                   = List->free;
   List->free                     = List->Element[List->free].next;
   List->Element[List->head].prev = NewIndex;
   List->Element[NewIndex].data   = Elem;
   List->Element[NewIndex].prev   = 0;
   List->Element[NewIndex].next   = List->head;
   List->head                     = NewIndex;
   List->size++;

   List->Element[0].prev = 0;

   if (List-> size >= List->MaxSize - 3)
      AddMemList(List);
   
   Check(NewIndex);

   AssertList(List);

   return true;
}

bool PushEnd(MyList *List, Elem_t Elem)
{
   AssertList(List);

   int NewIndex                   = List->free;
   List->free                     = List->Element[List->free].next;
   List->Element[List->tale].next = NewIndex;
   List->Element[NewIndex].data   = Elem;
   List->Element[NewIndex].next   = 0;
   List->Element[NewIndex].prev   = List->tale;
   List->tale                     = NewIndex;
   List->size++;

   List->Element[0].next = 0;

   if (List-> size >= List->MaxSize - 3)
      AddMemList(List);

   Check(NewIndex);

   AssertList(List);

   return true;
}

bool InsertAfter (MyList *List, int index, Elem_t Elem)
{
  AssertList(List);

  int NewIndex = List->free;
  List->free   = List->Element[List->free].next;

  List->Element[NewIndex].data                  = Elem;
  List->Element[NewIndex].next                  = List->Element[index].next;
  if (List->tale != index)
    List->Element[List->Element[index].next].prev = NewIndex;
  List->Element[index].next                     = NewIndex;
  List->Element[NewIndex].prev                  = index;
  List->size++;


  Check(NewIndex);

  if (List-> size >= List->MaxSize - 3)
      AddMemList(List);

  AssertList(List);

  return true;
}

bool InsertBefore (MyList *List, int index, Elem_t Elem)
{
  AssertList(List);

  int NewIndex = List->free;
  List->free   = List->Element[List->free].next;

  List->Element[NewIndex].data                  = Elem;
  List->Element[NewIndex].next                  = index;
  List->Element[NewIndex].prev                  = List->Element[index].prev;
  if (List->head != index)
    List->Element[List->Element[index].prev].next = NewIndex;
  List->Element[index].prev                     = NewIndex;
  List->size++;

  if (List-> size >= List->MaxSize - 3)
      AddMemList(List);

  Check(index);
  Check(NewIndex);

  AssertList(List);

  return true;

}

bool AddMemList (MyList *List)
{
    AssertList(List)

    List->Element = (ListElem*) realloc(List->Element, List->MaxSize * 2 * sizeof(ListElem));
    if (List->Element == nullptr)
        List->ErrorCode = EADDMEM;

    for (size_t index = List->MaxSize - 1; index < List->MaxSize * 2 - 1; index++){
         List->Element[index].data  = nullptr;
         List->Element[index].next = (int) index + 1;
         List->Element[index].prev = -1;
    }

    List->MaxSize *= 2;

    List->Element[List->MaxSize - 1].next = 0;
    List->Element[List->MaxSize - 1].prev = -1;

    
    AssertList(List);

    return true;
}

bool Delete (MyList *List, int index)
{
    AssertList(List);

    List->Element[List->Element[index].next].prev = List->Element[index].prev;
    List->Element[List->Element[index].prev].next = List->Element[index].next;
    List->Element[index].data                     = nullptr;
    Check(index);
    List->Element[index].next                     = List->free;
    List->Element[index].prev                     = -1;
    List->free = index;
    List->size--;

    AssertList(List);

    return true;
}

bool DestructList (MyList *List)
{
    AssertList(List);
  
    for (int index = 0; index < List->size; index++){
        List->Element[index].data = nullptr;
    }
    free(List);
  
    return true;
}

bool ClearList(MyList *List)
{
    for (int index = 0; index< InitSize; index++)
        List->Element[index].data = nullptr;

    List->head      = 0;
    List->tale      = 0;
    List->free      = 1;
    List->ErrorCode = NOERRORS;
    List->size      = 0;

    for (int IndexForFree = 1; IndexForFree < InitSize - 1; IndexForFree++)
        List->Element[IndexForFree].next = IndexForFree + 1;
    List->Element[List->MaxSize - 1].next = 0;
    for (int IndexForFree = 2; IndexForFree < InitSize; IndexForFree++)
        List->Element[IndexForFree].prev = -1;

    return true;
}

bool ListOK(MyList *List)
{
  assert(List !=nullptr);
  
  if (List->Element[0].data != nullptr) {List->ErrorCode     = EFIRSTELEM; return false;}
  if (List->Element[0].next != 0) {List->ErrorCode           = EFIRSTELEM; return false;}
  if (List->Element[0].prev != 0) {List->ErrorCode           = EFIRSTELEM; return false;}
  if (List->Element[List->tale].next != 0) {List->ErrorCode  = ETALE;      return false;}
  if (List->Element[List->head].prev != 0) {List->ErrorCode  = EHEAD;      return false;}
  if (List->ErrorCode != NOERRORS) {                                       return false;}

  return true;
}

void DUMP (MyList* List)
{
    printf("-----------------------------[DUMP]-----------------------------\n");
    printf("This is Dump, congratulations, you are fucked up somewhere...\n");
    printf("----------------------(Begin)\n");
    switch (List->ErrorCode) {
        case -1:   {printf("No errors\n\n");                              break;}
        case 1: {printf("Something wrong with zero element\n\n");         break;}
        case 100: {printf("Error occurred with Tale\n\n");                break;}
        case 11: {printf("Error occurred with Head\n\n");                 break;}
        default:  {printf("Something wrong with the code of error\n\n");  break;}
    }
    for (int i = 0; i < List->MaxSize; i++){
    printf("Element [%d] is %s     ", i, List->Element[i].data);
    printf("Next [%d] is %d     ", i , List->Element[i].next);
    printf("Prev [%d] is %d\n", i ,List->Element[i].prev);}
    printf("\n");
    printf("Free`s index - %d\n", List->free);
    printf("Head - %d\n", List->head);
    printf("Tale - %d\n", List->tale);
    printf("The maximum size of List - %zu\n", List->MaxSize);
    printf("Size - %zu\n",List->size);
    printf("\n");
    printf("(End)----------------------\n");
    printf("End of Dump. Good luck in searching of error\n");
    printf("And may the source be with you!\n");
    printf("----------------------------------------------------------------\n");
    printf("\n\n\n");
    //getchar();
}

