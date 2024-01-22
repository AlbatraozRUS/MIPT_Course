#include "Stack.h"

const char *FileForDump = "/Users/albatraozrus/Desktop/Dump.txt";

bool StackConstruct (MyStack_t *stk)
{
    stk->LitmusBegin       = LitmusBeginValue;
    stk->LitmusEnd         = LitmusEndValue;
    stk->size              = 0;
    stk->MaxSize           = 0;
    stk->HashSum           = 0;
    stk->ErrorCode         = 0;
    stk->data              = nullptr;
    stk->value             = nullptr;
    stk->ptrLitmusBegin    = nullptr;
    stk->ptrLitmusEnd      = nullptr;

    return true;
}

bool StackFirstInit (MyStack_t *stk)
{
    stk->data            = (char *) calloc(stk->MaxSize * sizeof(Elem_t) + 2 * sizeof(Litmus_t), sizeof(char));
    stk->size            = 1;
    stk->MaxSize         = 3;
    stk->value           = (Elem_t *) (stk->data + sizeof(Litmus_t));
    stk->ptrLitmusBegin  = (Litmus_t *) stk->data;
    *stk->ptrLitmusBegin = LitmusBeginValue;
    stk->ptrLitmusEnd    = (Litmus_t *) stk->data + sizeof(Litmus_t) + sizeof(Elem_t) * stk->MaxSize;
    *stk->ptrLitmusEnd   = LitmusEndValue;
    CalcHashSum

    return true;
}

bool StackPush(MyStack_t *stk, Elem_t elem)
{
    if(stk->MaxSize == 0)
       StackFirstInit(stk);

    assertMyStack(stk);

    if (stk->size >= stk->MaxSize) {
        stk->data = StackAddMem(stk, stk->MaxSize);
        StackDataLitmus(stk);
    }
    stk->value[stk->size++] = elem;
    CalcHashSum

    assertMyStack(stk);

    return true;

}

Elem_t StackPop(MyStack_t *stk)
{
    assertMyStack(stk);

    if (stk->size < 1) {
        stk->ErrorCode = EUNDERFLOW;
        assertMyStack(stk);
    }

    Elem_t value = stk->value[--stk->size];
    CalcHashSum
    assertMyStack(stk);

    if (stk->size < (stk->MaxSize / 3)) {
        stk->data = StackDecMem(stk, stk->MaxSize);
        StackDataLitmus(stk);
    }
    assertMyStack(stk);

    return value;
}

char* StackAddMem (MyStack_t* stk, size_t size)
{
    assertMyStack(stk);

    char *data = nullptr;
    while (size >= stk->MaxSize) {
        data = (char *) realloc(stk->data, sizeof(Litmus_t) * 2 + size * 2 * sizeof(Elem_t));
        if (data == nullptr) {
            stk->ErrorCode = EADDMEM;
            assertMyStack(stk);
        }
            stk->MaxSize *= 2;
    }
    StackDataLitmus(stk);

    assertMyStack(stk);

    return data;
}

char* StackDecMem (MyStack_t* stk, size_t size)
{
    assertMyStack(stk);

    char *data = nullptr;
    while (stk->size < (stk->MaxSize / 3)) {
        data = (char *) realloc(stk->data, (size / 2 + 1) * sizeof(Elem_t) + 2 * sizeof(Litmus_t));
        if (data == nullptr) {
            stk->ErrorCode = EADDMEM;
            assertMyStack(stk);
        }
        stk->MaxSize /= 2;
    }
    StackDataLitmus(stk);

    assertMyStack(stk);

    return data;
}

bool StackDataLitmus(MyStack_t*stk)
{
    stk->ptrLitmusBegin    = (Litmus_t *) stk->data;
    *stk->ptrLitmusBegin   = stk->LitmusBegin;
    stk->ptrLitmusEnd      = stk->value + sizeof(Litmus_t)*stk->MaxSize;
    *stk->ptrLitmusEnd     = stk->LitmusEnd;
    CalcHashSum

    assertMyStack(stk);

    return true;
}

bool StackDestruct(MyStack_t * stk)
{

    for (int i = 1; i < (stk->size); i++) {
        stk->value[i] = Poison;
    }

    free(stk->data);

    return true;
}

bool StackOK(MyStack_t *stk)
{
    assert(stk);

    if (stk->data == nullptr){stk->ErrorCode                                     = DATANULLPTR;        return false;}
    if (stk->value == nullptr){stk->ErrorCode                                    = DATANULLPTR;        return false;}
    if (stk->MaxSize < 1){stk->ErrorCode                                        = MAXSIZELESS0;       return false;}
    if (stk->size < 1){stk->ErrorCode                                            = SIZELESS0;          return false;}
    if (stk->size > stk->MaxSize) {stk->ErrorCode                                = SIZEMOREMAXSIZE;    return false;}
    if (stk->LitmusBegin != LitmusBeginValue){stk->ErrorCode                     = ELITMUSBEGIN;       return false;}
    if (stk->LitmusEnd != LitmusEndValue){stk->ErrorCode                         = ELITMUSEND;         return false;}
    if (*stk->ptrLitmusBegin != LitmusBeginValue){stk->ErrorCode                 = ELITMUSBEGIN;       return false;}
    if (*stk->ptrLitmusEnd != LitmusEndValue){stk->ErrorCode                     = ELITMUSEND;         return false;}
    if (stk->ptrLitmusBegin == nullptr){stk->ErrorCode                           = ELITMUSBEGIN;       return false;}
    if (stk->ptrLitmusEnd == nullptr){stk->ErrorCode                             = ELITMUSEND;         return false;}
    if (stk->HashSum != MurmurHash((char*) stk->value,
                                    sizeof(Elem_t) * stk->size)){stk->ErrorCode  = EHASHSUM;           return false;}
    if (stk->ErrorCode != 0){                                                                          return false;}

    return true;
}

void Dump(MyStack_t *stk)
{
    FILE * Dump = fopen(FileForDump, "w");
    fprintf(Dump, "WARNING!\n THERE IS AN ERROR IN THE STACK\n");
    fprintf(Dump, "Here is information about ERROR\n");
    fprintf(Dump, "-------------------------------------------\n\n");
    fprintf(Dump, "MyStack_t \"stk\"");
    fprintf(Dump, "[%p]\n", stk);
    fprintf(Dump, "The error: ");
    switch (stk->ErrorCode) {
        case 0:   {fprintf(Dump, "No errors\n");                                        break;}
        case 111: {fprintf(Dump, "Stack is Under Flowed\n");                            break;}
        case 112: {fprintf(Dump, "Error occurred while adding memory\n");               break;}
        case 211: {fprintf(Dump, "The pointer to data is nullptr\n");                   break;}
        case 311: {fprintf(Dump, "Size of stack is less than zero\n");                  break;}
        case 312: {fprintf(Dump, "The maximum capacity of Stack is less than zero\n");  break;}
        case 313: {fprintf(Dump, "The size is more bigger than maximum capacity\n");    break;}
        case 411: {fprintf(Dump, "The Litmus in the beginning is damaged\n");           break;}
        case 422: {fprintf(Dump, "The Litmus in the ending is damaged\n");              break;}
        case 511: {fprintf(Dump, "Error in the calculating Hash\n");                    break;}
        default:  {fprintf(Dump, "Something wrong with the code of error\n");           break;}
    }

    fprintf(Dump, "The size of Stack is %zu\n", stk->size - 1);
    fprintf(Dump, "The maximum size of Stack is %zu\n{\n", stk->MaxSize);
    fprintf(Dump, "The poison is %d\n", Poison);
    fprintf(Dump, "The value if Litmus in the beginning should be %d\n", LitmusBeginValue);
    fprintf(Dump, "Stack`s Litmus in the beginning is %d\n\n\n", *stk->ptrLitmusBegin);
    for (int i = 1; i < (stk->size); i++){
        fprintf(Dump, "Element %d of Stack is " Elem_TSpec "\n", i, stk->value[i]);
    }
    fprintf(Dump, "\n\nThe value if Litmus in the ending should be %d\n", LitmusEndValue);
    fprintf(Dump, "Stack`s Litmus in the ending is %d\n", *stk->ptrLitmusEnd);
    fprintf(Dump, "\n}\n\n-------------------------------------------");
    fclose(Dump);
}

