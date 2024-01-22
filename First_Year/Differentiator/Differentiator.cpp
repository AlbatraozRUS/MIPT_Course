#include "Differentiator.h"

bool EASY          = false;
Branch *Beginning  = nullptr;
FILE *Output       = nullptr;

bool FillTree (Branch *Node, Line *Text, unsigned int *Letter)
{
   assert(Node);
   assert(Text);

   unsigned int ScannedLetters = 0;
   char *String = (char*) calloc(SizeOfString, sizeof(char));
   int Number = POISON;

   for (;;)
   {
        if (*(Text->ptrbuffer + *Letter) == '('){
            *Letter += 1;
            if (Node->Left != nullptr && Node->Left->Elem->Type != POISON){
                Node->Right = CreateBranch(Node);
                FillTree(Node->Right, Text, Letter);
            }
            if (Node->Left == nullptr){
                Node->Left  = CreateBranch(Node);
                FillTree(Node->Left, Text, Letter);
            }
        }

        if (*(Text->ptrbuffer + *Letter) == ')'){
            *Letter += 1;
            FillTree(Node->Parent,Text, Letter);
        }

        Analyze(Text, Letter, Node, &Number, String, &ScannedLetters);

        if (*(Text->ptrbuffer + *Letter) != '\0')
            FillTree(Node, Text, Letter);

        break;
   }

   free(String);

   return true;
}


void Analyze (Line *Text, unsigned *Letter, Branch *Node, int *Number,
        char *String, unsigned *ScannedLetters)
{
          if (isdigit(*(Text->ptrbuffer + *Letter)) != 0){
             sscanf(Text->ptrbuffer + *Letter, "%d", Number);
             while (isdigit(*(Text->ptrbuffer + *Letter)) != 0)
               *Letter += 1;
             FillBranch(Node, MODE_NUMBER, nullptr, *Number);
             return;
          }
          if (*(Text->ptrbuffer + *Letter) == '+' || *(Text->ptrbuffer + *Letter) == '-' ||
                  *(Text->ptrbuffer + *Letter) == '*' || *(Text->ptrbuffer + *Letter) == '/' ||
                  *(Text->ptrbuffer + *Letter) == '^'){
              FillBranch(Node, MODE_OPERATOR, nullptr, *(Text->ptrbuffer + *Letter));
              *Letter += 1;
              FillTree(Node, Text, Letter);
              return;
          }

          if (*(Text->ptrbuffer + *Letter) == 'x'){
            FillBranch(Node, MODE_VARIABLE, nullptr, 'x');
            *Letter += 1;
            FillTree(Node, Text, Letter);
              return;
          }

          if (isalpha(*(Text->ptrbuffer + *Letter)) != 0){
            sscanf(Text->ptrbuffer + *Letter, "%[a-z]%n", String, ScannedLetters);
            FillBranch(Node, MODE_FUNCTION, String, POISON);
            *Letter += *ScannedLetters;
            Node->Left = CreateBranch(Node);
            FillBranch(Node->Left, NIL, nullptr, '$');
            return;
          }

}

Branch *Diff (Branch *Node)
{

    assert(Node);

    Beginning = Node;
    Sentences(Output);
    NextEquation(Output, Beginning);

    switch (Node->Elem->Type)
    {
    default:{fprintf(stderr, "Invalid type of function, please try again\n");}

    case MODE_NUMBER  : return NewBranch(MODE_NUMBER, nullptr, 0, nullptr, nullptr);

    case MODE_VARIABLE: return NewBranch(MODE_NUMBER, nullptr, 1, nullptr, nullptr);

    case MODE_OPERATOR: {
        switch (Node->Elem->ElemData)
        {
        case '+': return ADDDIFF(Node->Left, Node->Right);

        case '-': return SUBDIFF(Node->Left, Node->Right);

        case '*': return MULDIFF(Node->Left, Node->Right);

        case '/': return DIVDIFF(Node->Left, Node->Right);

        case '^': return PAWDIFF(Node->Left, Node->Right);

        default: break;
        }
        
     }

    case MODE_FUNCTION: return DiffFunction(Node);
    }


    fprintf(stderr, "Something wrong with diff function!\n");
    abort();

}

Branch *DiffFunction (Branch *Node)
{
    assert(Node);

    #define FUNCTION(name, code) {\
     if (strcmp(Node->Elem->Function, #name) == 0) {return {code};}\
        }

    #include "Commands.h"

    return nullptr;
}

bool MakeSimple (Branch *Node)
{
    if (Node->Left != nullptr)
        MakeSimple(Node->Left);
    if (Node->Right != nullptr)
        MakeSimple(Node->Right);

    while (!EASY)
        MakeEasier(Node);
    EASY = false;

    return true;
}

void MakeEasier (Branch *Node)
{
    if (Node->Elem->Type == MODE_OPERATOR){
        switch (Node->Elem->ElemData){
        default: break;
        case '*': {if (Node->Left->Elem->ElemData == 0 || Node->Right->Elem->ElemData == 0){
                       EASY = false;
                       FillBranch(Node, MODE_NUMBER, nullptr, 0);
                       Node->Left  = nullptr;
                       Node->Right = nullptr;
                       return;}
                   if (Node->Left->Elem->ElemData == 1){
                       EASY = false;
                       FillBranch(Node, Node->Right->Elem->Type,Node->Right->Elem->Function, Node->Right->Elem->ElemData);
                       ChangeChilds(Node, 'R');
                       return;}

                   if (Node->Right->Elem->ElemData == 1){
                       EASY = false;
                       FillBranch(Node, Node->Left->Elem->Type,Node->Left->Elem->Function, Node->Left->Elem->ElemData);
                       ChangeChilds(Node, 'L');
                       return;}
                   break;}
        case '+':{if (Node->Left->Elem->ElemData == 0){
                       EASY = false;
                       FillBranch(Node, Node->Right->Elem->Type,Node->Right->Elem->Function, Node->Right->Elem->ElemData);
                       ChangeChilds(Node, 'R');
                       return;}
                  if (Node->Right->Elem->ElemData == 0){
                       EASY = false;
                       FillBranch(Node, Node->Left->Elem->Type,Node->Left->Elem->Function, Node->Left->Elem->ElemData);
                       ChangeChilds(Node, 'L');
                       return;}
                  break;}
        case '-':{if (Node->Right->Elem->ElemData == 0){
                       EASY = false;
                       FillBranch(Node, Node->Left->Elem->Type,Node->Left->Elem->Function, Node->Left->Elem->ElemData);
                       ChangeChilds(Node, 'L');
                       return;}
                  break;}
        case '^':{if (Node->Right->Elem->ElemData == 0){
                       EASY = false;
                       FillBranch(Node, MODE_NUMBER, nullptr, 1);
                       Node->Left  = nullptr;
                       Node->Right = nullptr;
                       return;}
                  if (Node->Right->Elem->ElemData == 1){
                        EASY = false;
                        FillBranch(Node, Node->Left->Elem->Type,Node->Left->Elem->Function, Node->Left->Elem->ElemData);
                        ChangeChilds(Node, 'L');
                        return;}
                  break;}
        case '/':{if (Node->Left->Elem->ElemData  == 0){
                       EASY = false;
                       FillBranch(Node, MODE_NUMBER, nullptr, 0);
                       Node->Left  = nullptr;
                       Node->Right = nullptr;
                       return;}
                  break;}
        }
    }

    EASY = true;
    }

bool CheckForLeafs(Branch *Node)
{
    return Node->Left != nullptr && Node->Right != nullptr;
}

void ChangeChilds (Branch *Node, int Child)
{
    if (Child == 'L'){
        if (CheckForLeafs(Node->Left)){
            Node->Left  = Node->Left->Left;
            Node->Right = Node->Left->Right;
        }
        else {
            Node->Left = nullptr;
            Node->Right = nullptr;
        }
    }
    else {
        if (CheckForLeafs(Node->Right)){
            Node->Left  = Node->Right->Left;
            Node->Right = Node->Right->Right;
        }
        else {
            Node->Left = nullptr;
            Node->Right = nullptr;
        }
    }
}
