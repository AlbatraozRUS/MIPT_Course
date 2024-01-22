#include "Differentiator.h"

void print_mode(Branch* pos, FILE* dot_out) {
    assert(pos);
    assert(dot_out);

    switch ((pos->Elem)->Type) {
        case MODE_FUNCTION:     fprintf(dot_out, "[label = \"%s\", shape = \"hexagon\", "
                                           "color=\"#008000\", style=\"filled\", "
                                           "fillcolor = \"#32CD32\"]", (pos->Elem)->Function);
                          break;

        case MODE_OPERATOR:   fprintf(dot_out, "[label = \"%c\", shape = \"circle\", "
                                           "color=\"#00FFFF\", style=\"filled\", "
                                           "fillcolor = \"#F0E68C\"]", (pos->Elem)->ElemData);
                          break;

        case MODE_VARIABLE:    fprintf(dot_out, "[label = \"%c\", shape = \"diamond\", "
                                           "color=\"#FFFF00\", style=\"filled\", "
                                           "fillcolor = \"#9932CC\"]", (pos->Elem)->ElemData);
                          break;

        case MODE_NUMBER: fprintf(dot_out, "[label = \"%d\", shape = \"egg\", "
                                           "color=\"#FF0000\", style=\"filled\", "
                                           "fillcolor = \"#FFA07A\"]", (pos->Elem)->ElemData);
                          break;

        case NIL:         fprintf(dot_out, "[label = \"$\", shape = \"house\", "
                                           "color=\"#000000\", style=\"filled\", "
                                           "fillcolor = \"#FFA07A\"]");
                          break;

        default:          fprintf(dot_out, "\"error\"");
                          break;
    }

    assert(pos);
    assert(dot_out);
}


int tmp = 0;
void create_tree(Branch* pos, FILE* dot_out) {
    assert(dot_out);
    assert(pos);

    fprintf(dot_out, "\t\tNIL%d", tmp);
    print_mode(pos, dot_out);
    fprintf(dot_out, ";\n");

    int iskan = tmp;

    if (pos->Left != nullptr) {
        fprintf(dot_out, "\t\tNIL%d", iskan);

        fprintf(dot_out, "->");

        fprintf(dot_out, "NIL%d;\n", ++tmp);

        create_tree(pos->Left, dot_out);
    }

    if (pos->Right != nullptr) {
        fprintf(dot_out, "\t\tNIL%d", iskan);

        fprintf(dot_out, "->");

        fprintf(dot_out, "NIL%d;\n", ++tmp);

        create_tree(pos->Right, dot_out);
    }

    assert(pos);
    assert(dot_out);
}

void tree_dot(Branch* nameTree) {

    FILE *file_dot = fopen(DOT, "wb");
    assert(file_dot);


    fprintf(file_dot, "digraph {\n");
    fprintf(file_dot, "\tnode[color = \"#000000\", fontsize = 20];\n"
                      "\tedge[color = \"#000000\", fontsize = 20];\n");

    create_tree(nameTree, file_dot);

    fprintf(file_dot, "}");

    assert(file_dot);
    fclose(file_dot);

    system("dot -Tjpg -o /Users/albatraozrus/Desktop/Diff.jpg /Users/albatraozrus/Desktop/GraphwyzDiff.dot");

}
