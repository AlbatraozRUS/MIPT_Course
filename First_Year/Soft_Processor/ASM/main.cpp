#include "ASM.h"


int main() {

    printf("Welcome to the first prototype of ASM compiler\n");

    printf("Processing\n");
    size_t amount = 0;
    char *bytecode = (char*) calloc(10000 , sizeof(char));
    int PC = Assembler(bytecode, &amount);
    Writing(bytecode, PC);
    printf("...\n");
    printf("Process finished successfully\n");

    free(bytecode);
    return 0;
}
