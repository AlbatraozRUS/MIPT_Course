#include "Differentiator.h"

Branch *CreateBranch (Branch *Parent)
{

    struct Branch *NewNode = (Branch*) calloc(1, sizeof(Branch));

    if (Parent != nullptr)
        NewNode->Parent = Parent;
    
    return NewNode;

}

bool FillBranch (Branch *Node, unsigned Type, char *String, int Number)
{
    assert(Node);

    Node->Elem = (Data*) calloc(1, sizeof(Data));

    switch (Type) {
    case MODE_NUMBER   : {Node->Elem->ElemData = Number;         Node->Elem->Type = MODE_NUMBER;   break;}
    case MODE_OPERATOR : {Node->Elem->ElemData = Number;         Node->Elem->Type = MODE_OPERATOR; break;}
    case MODE_VARIABLE : {Node->Elem->ElemData = 'x';            Node->Elem->Type = MODE_VARIABLE; break;}
    case MODE_FUNCTION : {Node->Elem->Function = strdup(String); Node->Elem->Type = MODE_FUNCTION;
                          Node->Elem->ElemData = Number;                                                                         break;}
    case NIL           : {                                       Node->Elem->Type = NIL;           break;}
    }

    return true;
}

Branch *NewBranch (unsigned Type, char *String, int Number, Branch *LeftNode, Branch *RightNode)
{
    Branch *NewNode = (Branch*) calloc(1, sizeof(Branch));

    NewNode->Left  = LeftNode;
    NewNode->Right = RightNode;

    if (LeftNode  != nullptr)
        LeftNode->Parent = NewNode;
    if (RightNode != nullptr)
        RightNode->Parent = NewNode;

    FillBranch(NewNode, Type, String, Number);

    return NewNode;
}

Branch *Copy (Branch *Node)
{
    Branch *NewNode = (Branch*) calloc(1, sizeof(Branch));
    FillBranch(NewNode, Node->Elem->Type, Node->Elem->Function, Node->Elem->ElemData);
    NewNode->Parent = Node->Parent;

    if (Node->Left != nullptr)
        NewNode->Left  = Copy(Node->Left);
    if (Node->Right != nullptr)
        NewNode->Right = Copy(Node->Right);

    return NewNode;
}

bool DestBranch (Branch *Node)
{
    free(Node->Elem);
    if (Node->Left != nullptr)
        DestBranch(Node->Left);
    if (Node->Right != nullptr)
        DestBranch(Node->Right);

    free(Node);

    return true;
}
