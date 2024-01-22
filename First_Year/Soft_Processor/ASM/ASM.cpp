#include "ASM.h"

int Assembler (char *bytecode, size_t *amount)
{
    struct Line *ptrstruct = Reading(amount);
    DeleteComments(ptrstruct, *amount);

    auto *Command = (char*) calloc(6, sizeof(char));
    char Command_Number = 0;
    int PC = 0;
    unsigned int Param = Poison;
    int CountLabels = 0;
    struct Marks *Labels = (struct Marks*) calloc(AmountOfLabels, sizeof(int) + sizeof(char));
    assert(Labels !=nullptr);

    for (int index = 0; index < AmountOfLabels; index++)
    Labels[index].NameLabel = (char*) calloc(20, sizeof(char));

    Compilation(&Command_Number, &PC, &CountLabels, Labels,
                ptrstruct, Command, bytecode, &Param);
    PC = 0;
    Command_Number = -2;
    Compilation(&Command_Number, &PC, &CountLabels, Labels,
                ptrstruct, Command, bytecode, &Param);

    free(Labels);
    free(ptrstruct);
    free(Command);
    return PC;
}

char WhichCommand (char *Command)
{
    assert(Command!= nullptr);

  #define CMD_COMPARE(name, num, processor) \
    { if (strcasecmp(Command, #name) == 0)\
    return (char)(num);\
    }

    #define COMMANDS
    #include "Commands.h"

    #undef COMMANDS
    #undef CMD_COMPARE

    return ErrorWhichCommand;
}


void DeleteComments (struct Line *ptrstruct, size_t amount)
{
    char *temp = nullptr;

    for (int NumberOfLine = 0; NumberOfLine < amount; NumberOfLine++) {
        if ((temp = strchr(ptrstruct[NumberOfLine].ptrbuffer, ';')) != nullptr)
            *temp = '\0';
    }
}

bool PassTrash(char *Command)
{
    return strlen(Command) == 0;
}

bool ScanCommand (char *Command, struct Line *ptrstruct, int NumberOfLine)
{
    *Command = 0;
    sscanf(ptrstruct[NumberOfLine].ptrbuffer, "%s", Command);

    return true;
}

int WhichSpec (struct Line *ptrstruct, int NumberOfLine)
{
    int Letter = 0;
    while (isalpha(ptrstruct[NumberOfLine].ptrbuffer[Letter]))
        Letter++;
    if (isnumber(ptrstruct[NumberOfLine].ptrbuffer[Letter + 1]) ||
            ptrstruct[NumberOfLine].ptrbuffer[Letter + 1] == '-')
        return 1;
    if (isalpha(ptrstruct[NumberOfLine].ptrbuffer[Letter + 1]) &&
                ptrstruct[NumberOfLine].ptrbuffer[Letter] == ' ')
        return 2;
    return 3;
}

void ScanParam (int Spec, struct Line *ptrstruct, int NumberOfLine,unsigned int *Param, int *PC, char *bytecode)
{
    switch (Spec){
        case 1: {sscanf(ptrstruct[NumberOfLine].ptrbuffer, "%*s %d",        Param);
                *(unsigned int*)(bytecode + *PC) = *Param;  *PC += sizeof(int);  break;}
        case 2: {sscanf(ptrstruct[NumberOfLine].ptrbuffer, "%*s %c", (char*)Param);
                *(bytecode + *PC) = *(char*)Param; *PC += sizeof(char); break;}
        case 3: {                                  *PC += sizeof(char); break;}

        default: break;
    }
}

void FullBytecode (int *PC, char *bytecode, char *Command_Number, char *Command,
                   int NumberOfLine, struct Line *ptrstruct, unsigned int *Param, struct Marks *Lables, int CountLables)
{
    *Command_Number = WhichCommand(Command);
    bytecode[*PC] = *Command_Number;
    *PC += sizeof(char);

    #define JUMPS
    #define CMD_COMPARE(name, num)\
    {\
    if (*Command_Number == (num)){\
        TransformName(ptrstruct, NumberOfLine, Lables, CountLables, bytecode, PC);\
        return;\
        }\
    }
    #include "Commands.h"
    #undef JUMPS
    #undef CMD_COMPARE

    if (*Command_Number == 100){
        TransformName(ptrstruct, NumberOfLine, Lables, CountLables, bytecode, PC);
        return;
    }
    bytecode[*PC] = (char) WhichSpec(ptrstruct, NumberOfLine);
    *PC += sizeof(char);
    *Param = Poison;
    ScanParam (bytecode[*PC - 1],ptrstruct, NumberOfLine, Param, PC, bytecode);
}

bool LocateLabels(struct Line *ptrstruct, int NumberOfLine, struct Marks *Labels, int *CountLabels, int *PC)
{
    if (strchr(ptrstruct[NumberOfLine].ptrbuffer, ':') == nullptr)
        return false;

    size_t Letter = 0;
    while (isalpha(ptrstruct[NumberOfLine].ptrbuffer[Letter]))
        Letter++;
    ptrstruct[NumberOfLine].ptrbuffer[Letter + 1] = '\0';
    strncpy(Labels[*CountLabels].NameLabel, ptrstruct[NumberOfLine].ptrbuffer,  Letter);
    Labels[*CountLabels].Address = *PC;
    *CountLabels += 1;

    return true;
}

void TransformName(struct Line *ptrstruct, int NumberOfLine, struct Marks *Lables,
                  int CountLables, char *bytecode, int *PC)
{
   char *NameLable = (char*) calloc(15, sizeof(char));
   sscanf(ptrstruct[NumberOfLine].ptrbuffer, "%*s %s", NameLable);

   for (int NumberLable = 0; NumberLable < CountLables; NumberLable++){
       if (strcmp(NameLable, Lables[NumberLable].NameLabel) == 0){
        *(int*)(bytecode + *PC) = Lables[NumberLable].Address;
       }
   }
   free(NameLable);
   *PC += sizeof(int);
}

void Compilation (char *Command_Number, int *PC, int *CountLabels, struct Marks *Labels,
                  struct Line *ptrstruct, char *Command, char *bytecode, unsigned int *Param)
{

  for (int NumberOfLine = 0; *Command_Number != -1; NumberOfLine++){

        if (LocateLabels(ptrstruct, NumberOfLine, Labels, CountLabels, PC)){;
            continue;
        }

        ScanCommand(Command, ptrstruct, NumberOfLine);

        if (PassTrash(Command))
            continue;

        FullBytecode(PC, bytecode, Command_Number, Command, NumberOfLine, ptrstruct, Param, Labels, *CountLabels);
        //DumpASM(Command_Number, PC, ptrstruct, Command, bytecode, NumberOfLine);
    }
}
void DumpASM (char *Command_Number, int *PC, struct Line *ptrstruct, char *Command, char *bytecode, int NumberOfLine)
{

    printf ("-------------- [%02X] %s : %d -------------\n",*PC, Command, *Command_Number);
    const int codeSz = 32;
    printf("               [%d] String: %s\n", NumberOfLine + 1, ptrstruct[NumberOfLine].ptrbuffer);
    for (int i = 0; i < codeSz; i++) printf ("%02X ", i);
    printf ("\n");
    for (int i = 0; i < codeSz; i++) printf ("%02X ", (unsigned char) bytecode[i]);
    printf ("\n");
    for (int i = 0; i < codeSz; i++) printf ("%s ", (i == *PC - 3)? "^^" : "  ");


    printf ("\n-----------------------------------------------------------------------------------------------\n");
    fflush (stdout);

    getchar();
}
