#include "List.h"

void diagraph (MyList *List)
{    FILE *Dot = fopen ("/Users/albatraozrus/Desktop/Graphwyz.dot", "w");
    fprintf (Dot, "digraph {\nnode [shape=\"box\"]\n"
                  "edge [style=\"dashed\" dir=\"both\"]\n"
                  "dirtype = \"both\"\n"
                  "rankdir=\"LR\"\n");
    for (int Index = List->head; Index != 0; Index = List->Element[Index].next) {
        fprintf (Dot, "\telem_%d [label=<<TABLE BORDER=\"1\" CELLBORDER=\"3\" CELLSPACING=\"5\">\n"
                      "\t\t<TR><TD COLSPAN=\"3\" BGCOLOR=\"orange\"> idx: %d </TD></TR>\n"
                      "\t\t<TR><TD COLSPAN=\"3\" BGCOLOR=\"white\"> data: %lg </TD></TR>\n"
                      "\t\t<TR><TD BGCOLOR=\"red\" PORT=\"prev\"> prev: %d </TD>\n"
                      "\t\t<TD BGCOLOR=\"yellow\" PORT=\"next\"> next: %d </TD></TR>\n"
                      "\t</TABLE>>]\n", Index, Index, List->Element[Index].data,
                 List->Element[Index].prev,
                 List->Element[Index].next);
    }
    if (List->head != 0)
        fprintf (Dot, "\"data\" -> elem_%d\n", List->head);
    else
        fprintf (Dot, "\"data\" -> \"empty\"\n");
    for (int Index = List->head; List->Element[Index].next != 0; Index = List->Element[Index].next)
        fprintf (Dot, "elem_%d -> elem_%d\n", Index, List->Element[Index].next);

    fprintf (Dot, "}\n");
    fclose (Dot);
    system("dot -Tjpg -o /Users/albatraozrus/Desktop/TEMP.jpg /Users/albatraozrus/Desktop/Graphwyz.dot");
}
