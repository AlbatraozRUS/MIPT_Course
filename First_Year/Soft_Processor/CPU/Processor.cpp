#include <unistd.h>
#include "Processor.h"

void CPU_Reader (char *bytecode, MYCPU *CPU)
{
    assert(bytecode != nullptr);

    int PC = 0;
    int TempForIn = 0;
    double TempForDouble = 0;
    
#define CMD_COMPARE(name, num, processor){\
       case num: /*DumpCPU (CPU, #name, num, PC, bytecode);*/ {processor}\
    }

    for (;;) {
        switch (bytecode[PC]) {
            #define COMMANDS
            #include "Commands.h"
            default:
              break;
            #undef CMD_COMPARE
            #undef COMMANDS
        }
    }
}


void DumpCPU (const MYCPU* CPU, const char* cmdName, int cmdNum, int PC, const char* bytecode)
{
    assert (CPU); assert (cmdName); assert (bytecode);
    static int calls = 0; calls++;

    printf ("-------------- [%02X] %s : %d -------------\n", PC, cmdName, cmdNum);

    const int codeSz = 32;

    for (int i = 0; i < codeSz; i++){if (i % 45 == 0) printf("\n"); printf ("%02X ", i);}
    printf ("\n");
    for (int i = 0; i < codeSz; i++){if (i % 45 == 0) printf("\n"); printf ("%02X ", (unsigned char) bytecode[i]);}
    printf ("\n");
    for (int i = 0; i < codeSz; i++) printf ("%s ", (i == PC)? "^^" : "  ");
//    printf ("\n");

    printf ("-------------------------------------------\n");
    printf ("AX = %04X BX = %04X CX = %04X DX = %04X\n", CPU->CR[0], CPU->CR[1], CPU->CR[2], CPU->CR[3]);

    printf ("Stack: ");
    for (int i = 1; i < CPU->stk.size; i++) printf ("[%02X]=%04X, ", i, CPU->stk.value[i]);
    printf ("(end)\n------------------------------------------- pause...\n");
    fflush (stdout);

    //getchar();
    char c = 0;read (0, &c, sizeof (c));
    printf ("next... (%d Poltorashka's photos required)\n", calls);
}

