#include "Akinator.h"
#include "Tree.h"

int main()
{
    printf("Welcome to Akinator!\nLet me guess your character\n");
    printf("So, lets begin\n\n\n");

    struct Branch Root = {};
    struct Tree TreeAkin = {};
    struct Line *Text = Reading();

    unsigned int Letter = 0;
    FillTree(&Root, Text, &Letter, &TreeAkin.size);
    TreeAkin.Head = Root.left;
    Menu(TreeAkin.Head, &TreeAkin.size);

    FILE *DotFile = fopen(DOT, "w");
    DotForAkinator(DotFile , TreeAkin.Head);
    fclose(DotFile);

    FILE *Output_Result = fopen(Input_File, "w");
    Writting(Root.left, Output_Result);
    fclose(Output_Result);

    printf("\n\nFINISHED SUCCESSFULLY!\n");

    return 0;
}
