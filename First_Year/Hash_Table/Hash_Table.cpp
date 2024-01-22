#include "List.h"
#include "Hash_Table.h"

struct MyList* InitHashTable()
{
    struct MyList *Table = (MyList*) calloc(SizeOfTable, sizeof(MyList));
    for (int NumTable = 0; NumTable < SizeOfTable; NumTable++){
        CreateList(&Table[NumTable]);
        InitList(&Table[NumTable]);
    }

    return Table;
}

bool FillHashTable(MyList *Table)
{
   size_t amount = 0;

   struct Line *Strings = Reading(&amount);
   FILE *Results = fopen("/Users/albatraozrus/Desktop/result.csv", "w");
   DecorateFile(Results);

   FILLHASHTABLE(Table, Strings, amount, NotReallyHash(),Results)
   FILLHASHTABLE(Table, Strings, amount, FirstLetter(Strings[Line].ptrbuffer),Results)
   FILLHASHTABLE(Table, Strings, amount, Length(Strings, Line),Results)
   FILLHASHTABLE(Table, Strings, amount, SumOfLetters(Strings, Line),Results)
   FILLHASHTABLE(Table, Strings, amount, Roll(Strings[Line].ptrbuffer),Results)
   FILLHASHTABLE(Table, Strings, amount, GNUHash(Strings[Line].ptrbuffer,
           Strings[Line].ptrbuffer[Strings[Line].length - 1]),Results)
   FILLHASHTABLE(Table, Strings, amount, MurmurHash(Strings[Line].ptrbuffer, Strings[Line].length),Results)
   fclose(Results);
   
   return true;
}

bool DecorateFile(FILE *Results)
{
  for (int index = 0; index < SizeOfTable; index++)
      fprintf(Results, "%d;", index);
  fprintf(Results, "\n");

    return true;
}

void HashDUMP(MyList *Table)
{
    printf("-----------------------------[DUMP]-----------------------------\n");
    printf("This is Dump, congratulations, you are fucked up somewhere...\n");
    printf("----------------------(Begin)\n");
    for (int NumTable = 0; NumTable < SizeOfTable; NumTable++){
    printf("LIST NUMBER - %d\n", NumTable);
    for (int i = 0; i < Table[NumTable].MaxSize; i++){
    printf("Element [%d] is %10s     ", i, Table[NumTable].Element[i].data);
    printf("Next [%d] is %d     ", i , Table[NumTable].Element[i].next);
    printf("Prev [%d] is %d\n", i ,Table[NumTable].Element[i].prev);}
    printf("Free`s index - %d\n", Table[NumTable].free);
    printf("Head - %d\n", Table[NumTable].head);
    printf("Tale - %d\n", Table[NumTable].tale);
    printf("The maximum size of List - %zu\n",Table[NumTable].MaxSize);
    printf("Size - %zu\n",Table[NumTable].size);
    printf("\n\n");
    }
    printf("\n\n");
    printf("(End)----------------------\n");
    printf("End of Dump. Good luck in searching of error\n");
    printf("And may the source be with you!\n");
    printf("----------------------------------------------------------------\n");
    printf("\n\n\n");
    //getchar();
}
