// Example of using List

#include "List.h"


int main()
{
    printf("Welcome to the List!\nOh Shit, here we go again\n");
    printf("Processing\n");
    printf("...\n");

    MyList List = {};

    CreateList(&List);
    InitList(&List);;

    PushBegin(&List, 1);
    PushBegin(&List, 2);
    PushBegin(&List, 3);
    DUMP(&List);
    ClearList(&List);
    PushBegin(&List, 4);
    PushBegin(&List, 5);
    PushBegin(&List, 6);
    DUMP(&List);


    printf("Finished successfully\n");

    //DUMP(&List);
    //diagraph(&List);

    DestructList(&List);

    return 0;
}
