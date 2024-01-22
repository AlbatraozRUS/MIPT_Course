#include "Differentiator.h"

void Sentences (FILE *Output)
{
    switch(rand() % 5){

    case 0: {fprintf(Output,"\nIt is obvious that\n"); break;}

    case 1: {fprintf(Output,"\nLet notice that\n"); break;}

    case 2: {fprintf(Output,"\nIt is easy to see that\n"); break;}

    case 3: {fprintf(Output,"\nIt turns out that\n"); break;}

    case 4: {fprintf(Output,"\nThen we get\n"); break;}

    default: {printf("Something wrong with writing sentences to Latex\n"); abort();}
    }
}

void NextEquation (FILE *Output, Branch *Node)
{
    assert(Output);
    assert(Node);

    fprintf(Output, "\n$");

    PrintLatex(Node, Output);

    fprintf(Output, "$\n");
}


void PrintLatex (Branch *Node, FILE *Output)
{
    if (!Node) return;

    if (Node->Elem->Type == MODE_OPERATOR){

        switch (Node->Elem->ElemData){
        case '/': {
            fprintf(Output, "\\frac{");

            PrintLatex(Node->Left, Output);
            fprintf(Output, "}{");

            PrintLatex(Node->Right, Output);
            fprintf(Output, "}");

            return;}

        case '^': {
                fprintf(Output, "(");
                PrintLatex(Node->Left, Output);
                fprintf(Output, ")");

                fprintf(Output, "^{");
                PrintLatex(Node->Right, Output);
                fprintf(Output, "}");

            return;}

        case '*': {
                  if (Node->Left->Elem->ElemData == '*' || Node->Left->Elem->ElemData == ':' ||
                      Node->Left->Elem->ElemData == '/' || Node->Left->Elem->Type == MODE_FUNCTION)

                     PrintLatex(Node->Left, Output);

                 else {
                     fprintf(Output, "(");
                     PrintLatex(Node->Left, Output);
                     fprintf(Output, ")");
                 }

                if (Node->Right->Elem->ElemData == '^' || Node->Right->Elem->ElemData == '*' ||
                    Node->Right->Elem->ElemData == '/' || Node->Right->Elem->Type == MODE_FUNCTION)
                PrintLatex(Node->Right, Output);

                else {
                    fprintf(Output, "(");
                    PrintLatex(Node->Right, Output);
                    fprintf(Output, ")");
                }

            return;}
        }
    }

    PrintLatex(Node->Left, Output);

    switch (Node->Elem->Type){

       case MODE_OPERATOR: {fprintf(Output, "%c", Node->Elem->ElemData); break;}

       case MODE_NUMBER:   {fprintf(Output, "%d", Node->Elem->ElemData); break;}

       case MODE_VARIABLE: {fprintf(Output, "%c", Node->Elem->ElemData); break;}

       case MODE_FUNCTION: {fprintf(Output, "\\%s(", Node->Elem->Function); PrintLatex(Node->Right, Output);
                            fprintf(Output, ")"); return;}

       case NIL:           {break;}


       default:            {printf("Type unrecognized in Print Latex\n Type - %d\n", Node->Elem->Type); abort();}
         }

    PrintLatex(Node->Right, Output);
}

void Latex (Branch *Node, FILE *Output)
{
    assert(Node);

    fprintf(Output, "\\documentclass{article}\n\n"
                 "\\title{Differentiator}\n"
                 "\\author{AlbatraozRUS}\n\n"
                 "\\begin{document}\n"
                 "\\maketitle\n\n"
                 "\\newpage\n"
                 "At first we have\n"
                 "$");

    PrintLatex(Node, Output);

    fprintf(Output, "$\n");
}

