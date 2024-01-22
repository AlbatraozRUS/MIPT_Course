// Example of using Stack

#include "Stack.h"

int main()
{
    
    MyStack_t stk = {};

    StackConstruct(&stk);

    StackPush(&stk, 111);
    StackPush(&stk, 112);
    StackPush(&stk, 113);
    StackPush(&stk, 114);
    StackPush(&stk, 115);
    //UnitTests(&stk);
    StackPush(&stk, 116);


    Elem_t temp1  = StackPop(&stk);
    Elem_t temp2  = StackPop(&stk);
    Elem_t temp3  = StackPop(&stk);
    Elem_t temp4  = StackPop(&stk);
    Elem_t temp5  = StackPop(&stk);
    //UnitTests(&stk);
    Elem_t temp6  = StackPop(&stk);

    std::cout << temp1 << "  ";
    std::cout << temp2 << "  ";
    std::cout << temp3 << "  ";
    std::cout << temp4 << "  ";
    std::cout << temp5 << "  ";
    std::cout << temp6 << "  ";

    StackDestruct(&stk);

    return 0;
}
