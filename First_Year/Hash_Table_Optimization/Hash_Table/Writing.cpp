#include "Hash_Table.h"

bool Writting(MyList *Table, FILE *Results)
{
    assert(Results);

    for (int index = 0; index < SizeOfTable; index++)
    {
        fprintf(Results, "%zu;", Table[index].size);
    }
    fprintf(Results, "\n");

    return true;
}
