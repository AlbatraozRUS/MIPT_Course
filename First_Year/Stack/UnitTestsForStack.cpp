#include "Stack.h"

void UnitTests(MyStack_t *stk)
{
    int Number = 0;
    printf("Please input a number for check from 1 to 8\n");
    scanf("%d", &Number);

    switch (Number) {
        case 1: { stk->LitmusBegin = -666;       break;}
        case 2: { stk->LitmusEnd = -666;         break;}
        case 3: { stk->ptrLitmusBegin = nullptr; break;}
        case 4: { stk->ptrLitmusEnd = nullptr;   break;}
        case 5: { stk->data = nullptr;           break;}
        case 6: { stk->MaxSize = 0;              break;}
        case 7: { stk->value = nullptr;          break;}
        case 8: { stk->HashSum = 0;              break;}
        default: printf("Please try to input number again\n");
    }
}