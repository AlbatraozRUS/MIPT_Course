#include "Differentiator.h"

extern Branch *Beginning;
extern FILE   *Output;

int main()
{
    printf("Welcome to Differentiator!\nLets begin!\n");
    struct Line *Buffer = Reading();
    struct Tree Root    = {};

//    unsigned Letter = 0;
//    Root.Head = CreateBranch(nullptr);
//    FillTree(Root.Head, Buffer, &Letter);
    Root.Head = GetG(Buffer->ptrbuffer);
    Beginning = Root.Head;
    Output = fopen(TEX_Output, "wb");
    Latex(Root.Head, Output);
    Branch *AfterDiff = Diff(Root.Head);
    //MakeSimple(AfterDiff);
    fprintf(Output, "\nIn the end we have the answer\n\n$");
    PrintLatex(AfterDiff, Output);
    fprintf(Output, "$\n\n\\end{document}");

    tree_dot(AfterDiff);

    fclose(Output);
    DestBranch(AfterDiff);
    DestBranch(Root.Head);


    printf("Program finished successfully!\n");
    return 0;
}
