#include "Akinator.h"


void FillDot (Branch* Node, FILE* dot_out)
{
    if (Node->left != nullptr) {
        fprintf (dot_out, "\n\t\t\"%s\"[shape = \"ellipse\", color=\"#191970\", style=\"filled\", fillcolor = \"#E0FFFF\"];\n"
                          "\t\t\"%s\"->\"%s\"[label = \"YES\"];\n", Node->data, Node->data, Node->left->data);

        FillDot(Node->left, dot_out);
    }

    if (Node->right != nullptr) {
        fprintf (dot_out, "\n\t\t\"%s\"[shape = \"egg\", color=\"#191970\", style=\"filled\", fillcolor = \"#E0FFFF\"];\n"
                          "\t\t\"%s\"->\"%s\"[label = \"NO\"];\n", Node->data, Node->data, Node->right->data);

        FillDot (Node->right, dot_out);
    }
}


void DotForAkinator (FILE* file_dot, Branch *Head)
{
    assert(file_dot);

    fprintf(file_dot, "digraph {\n");
    fprintf(file_dot, "\tnode[shape = \"octagon\", color = \"#191970\", fontsize = 12, style = \"filled\", fillcolor = \"#87CEFA\"];\n"
                      "\tedge[color = \"#191970\", fontsize = 14];\n");

    FillDot (Head, file_dot);

    fprintf(file_dot, "}");


    fclose(file_dot);

    system("dot -Tjpg -o /Users/albatraozrus/Desktop/Akinator.jpg /Users/albatraozrus/Desktop/GraphwyzAkin.dot");
    //system("open /Users/albatraozrus/Desktop/Akinator.jpg");
}
