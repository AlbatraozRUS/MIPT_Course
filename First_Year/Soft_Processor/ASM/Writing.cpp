#include "ASM.h"

void Writing (char *bytecode, int size)
{
    assert(bytecode != nullptr);

    FILE *Writefile = fopen(Output_File, "wb");
    assert(Writefile != nullptr);

    fwrite(bytecode, sizeof(bytecode[0]), (size_t)size, Writefile);
   /* for (int PC = 0; PC < size; PC++)
    printf("%02X ", bytecode [PC]);*/

    fclose(Writefile);
}
