#include "Akinator.h"

Branch *CreateBranch (Branch *Parent, char *data, unsigned int *Size)
{
    static int Number = 1;

    assert(Parent);

    struct Branch *NewNode = (Branch*) calloc(1, sizeof(Branch));

    NewNode->parent = Parent;

    NewNode->Number = Number;
    Number++;

    NewNode->data = strdup(data);

    *Size += 1;
    
    return NewNode;
}
