#include "Akinator.h"
Branch *WayToAnswer = nullptr;

bool FillTree (Branch *Node, Line *Text, unsigned int *Letter, unsigned int *Size)
{
   assert(Node);
   assert(Text);

   unsigned int ScannedLetters = 0;
   char *String = (char*) calloc(SizeOfString, sizeof(char));

   for (;;)
   {
       if (sscanf(Text->ptrbuffer + *Letter, "{ \"%[^\"]\" %n", String, &ScannedLetters) > 0){
           *Letter += ScannedLetters;
           Node->left = (CreateBranch(Node, String, Size));
           FillTree(Node->left, Text, Letter, Size);
       }

       while (Text->ptrbuffer[*Letter] == '}'){
           Node = Node->parent;
           *Letter += 2 * sizeof(char);
       }

       if (sscanf(Text->ptrbuffer + *Letter, "{ \"%[^\"]\" %n", String, &ScannedLetters) > 0){
           *Letter += ScannedLetters;
           Node->right = (CreateBranch(Node, String, Size));
           FillTree(Node->right, Text, Letter, Size);
       }
       break;
   }

   free(String);

   return true;
}

bool ExploreTree (Branch *Node, unsigned int *Size)
{
    assert(Node);
    unsigned Answer = 666;

    if (Node->right == nullptr && Node->left == nullptr){
        Result(Node, &Answer, Size);
        return true;
    }
    printf("%s\n", Node->data);
    printf("Yes - 1\nNo - 0\n");
    scanf("%d", &Answer);
    while (Answer != 0 && Answer != 1){
         printf("You have entered the wrong number\n");
         printf("Please try again\n");
         scanf("%d", &Answer);
    }
    if (Answer == 1)
        ExploreTree(Node->left, Size);
    if (Answer == 0)
        ExploreTree(Node->right, Size);
    return true;
}

bool Result(Branch *Node, unsigned *Answer, unsigned *Size)
{
    printf("Is your character?\n%s\n", Node->data);
    printf("Yes - 1\nNo - 0\n");
    scanf("%d", Answer);
     if (*Answer == 1){
         printf("I won again\n");
         return true;
     }
     if (*Answer == 0){
         AddNewAnswer(Node, Size);
         return true;
     }

    return false;
}

bool AddNewAnswer (Branch *Node, unsigned int *Size)
{
    assert(Node);

    printf("Looks like i do not know this character\n");
    struct Branch *NewNode = (Branch*) calloc(1, sizeof(Branch));
    NewNode->parent = Node;
    NewNode->data = (char*) calloc(SizeOfString, sizeof(char));
    char QWho[] = "Who is it?";
    NewNode->data = Dialogue(QWho);
    *Size += 1;

    Node->left  = NewNode;
    Node->right = CreateBranch(Node, Node->data, Size);
    char QWhat[] = "What is the difference between them?";
    Node->data = Dialogue(QWhat);

    return true;
}

char *Dialogue (char *Question)
{
    printf("%s\n", Question);
    char TempAns[SizeOfString] = {};
    scanf("\n%[^\n]", TempAns);
    char *Ans = strdup(TempAns);

    return Ans;
}

bool Writting (Branch *Node, FILE *Output_File)
{
   fprintf(Output_File, "{ \"%s\" ", Node->data);

   if (Node->left != nullptr)
       Writting(Node->left, Output_File);
   if (Node->left == nullptr)
       return true;
   if (Node->right != nullptr){
       fprintf(Output_File, "} ");
       Writting(Node->right, Output_File);
   }

   fprintf(Output_File, "} ");

   return true;
}

bool WayToElem (Branch *Node, char *String)
{
    if (strcmp(Node->data, String) != 0)
    {
        if (Node->left != nullptr)
            WayToElem(Node->left, String);
        if (Node->left == nullptr)
            return true;
        if (Node->right != nullptr)
            WayToElem(Node->right, String);
    }

    if (strcmp(Node->data, String) == 0)
        WayToAnswer = Node;

    return true;
}

bool PrintWay(Branch *Node, char *String)
{
    WayToAnswer = (Branch*) calloc(1, sizeof(Branch));
    WayToElem(Node, String);
    if (WayToAnswer->data == nullptr){
        printf("Character not found, please try again\n");
        abort();
    }
    while (WayToAnswer->parent != nullptr){
        printf("%s ", WayToAnswer->data);
        WayToAnswer = WayToAnswer->parent;
    }

    return true;
}

bool Compare (Branch *Root, char *FirstElement, char *SecondElement, unsigned int Size)
{
    WayToAnswer = (Branch*) calloc(1, sizeof(Branch));
    WayToElem(Root, FirstElement);
    if (WayToAnswer->data == nullptr){
        printf("Character %s not found, please try again\n", FirstElement);
        abort();
    }
    Branch *WAYONE = (Branch*) calloc(Size, sizeof(Branch));
    unsigned FAmount = 0;
    for (; WayToAnswer != Root->parent; FAmount++){
        WAYONE[FAmount] = *WayToAnswer;
        WayToAnswer = WayToAnswer->parent;
    }
    WayToElem(Root, SecondElement);
    if (WayToAnswer->data == nullptr){
        printf("Character %s not found, please try again\n", SecondElement);
        abort();
    }
    Branch *WAYTWO = (Branch*) calloc(Size, sizeof(Branch));
    unsigned SAmount = 0;
    for (; WayToAnswer != Root->parent; SAmount++){
       WAYTWO[SAmount] = *WayToAnswer;
       WayToAnswer = WayToAnswer->parent;
    }
    printf("Общее : ");
    FAmount--;
    SAmount--;
    while (strcmp(WAYONE[FAmount - 1].data,WAYTWO[SAmount - 1].data) == 0){
       if (WAYONE[FAmount - 1].Number == WAYONE[FAmount].right->Number)
            printf("Не %s, ", WAYONE[FAmount].data);
       else
            printf("%s, ", WAYONE[FAmount].data);
        FAmount--;
        SAmount--;
    }
    printf("\nНо по сравнению с %s персонаж %s\n", WAYTWO[0].data, WAYONE[0].data);
    while (WAYONE[FAmount].left != nullptr){
        if (&WAYONE[FAmount - 1] != nullptr){
            if (WAYONE[FAmount - 1].Number == WAYONE[FAmount].right->Number)
                printf("Не %s, ", WAYONE[FAmount].data);
            else
                printf("%s, ", WAYONE[FAmount].data);
            FAmount--;
        }
    }
    printf("\nА %s в свою очередь\n", WAYTWO[0].data);
    while (WAYTWO[SAmount].left != nullptr){
        if (&WAYTWO[SAmount - 1] != nullptr){
            if (WAYTWO[SAmount - 1].Number == WAYTWO[SAmount].right->Number)
                    printf("Не %s, ", WAYTWO[SAmount].data);
                else
                    printf("%s, ", WAYTWO[SAmount].data);
                SAmount--;
        }
    }
    printf("\n");
    free(WAYONE);
    free(WAYTWO);

    return true;
}

void Menu (Branch *Root, unsigned *Size)
{
  printf("What do you want to do?\n");
  printf("1 - Guess character\n2 - Compare two characters\n3 - Show definition\n");
  int Answer = -1;
  scanf("%d", &Answer);
  switch (Answer){
  case 1: {ExploreTree(Root, Size);
      return;}

  case 2: {char *StringOne = (char*) calloc(SizeOfString, sizeof(char));
            printf("Enter your first element\n");
            scanf("\n%[^\n]", StringOne);
            char *StringTwo = (char*) calloc(SizeOfString, sizeof(char));
            printf("\nEnter your second element\n");
            scanf("\n%[^\n]", StringTwo);
            Compare (Root, StringOne, StringTwo, *Size);
            free(StringOne);
            free(StringTwo);
      return;}

  case 3: {char *StringOne = (char*) calloc(SizeOfString, sizeof(char));
            printf("Enter your element\n");
            scanf("\n%[^\n]", StringOne);
            PrintWay(Root, StringOne);
            free(StringOne);
      return;}

      default: {printf("Command not recognized, please try again\n"); abort();}
  }
}
