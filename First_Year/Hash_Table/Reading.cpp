#include "List.h"
#include "Hash_Table.h"

long CounterOfSymbols(FILE * readfile)
{

    assert(readfile != nullptr);

    fseek(readfile , 0 , SEEK_END);
    long size = ftell(readfile);
    fseek(readfile , 0 , SEEK_SET );

    return size;
}

void SurgeryOfLines(char *buffer, long size){

    for (int i = 0; i < size; i++){
        if (buffer[i] == '\n'){
            buffer[i] = '\0';
        }
    }
}

size_t CounterOfLines(char *buffer, long size)
{

    assert(buffer != nullptr);

    size_t amount = 1;
    for (int index_1 = 0; index_1 < size; index_1++){
        if (buffer[index_1] == '\n') {
            if (buffer[index_1 + 1] == '\n')
                buffer[index_1 + 1] = ' ';
            amount += 1;
        }
    }
    return amount;
}

struct Line* CreateStruct(char *buffer, size_t size, size_t amount)
{
    assert(buffer != nullptr);

    struct Line *ptrstruct = (struct Line*) calloc(amount, sizeof(ptrstruct[0]));
    assert(ptrstruct != nullptr);

    ptrstruct[0].ptrbuffer = buffer;
    for (int letter = 0, NumberOfLine = 1; letter < size; letter++){
        if (buffer[letter] == '\0') {
            ptrstruct[NumberOfLine].ptrbuffer = &buffer[letter + 1];
            ptrstruct[NumberOfLine - 1].length = (unsigned int) (ptrstruct[NumberOfLine].ptrbuffer -
                                                                 ptrstruct[NumberOfLine - 1].ptrbuffer);
            NumberOfLine++;
        }
    }

    return ptrstruct;
}

struct Line * Reading(size_t *amount)
{

    FILE *readfile = fopen(Input_File , "rb" );

    long size = 0;


    size = CounterOfSymbols(readfile);

    char *buffer = (char*) calloc((size_t)size + 5100, sizeof(char));
    assert(buffer != nullptr);

    fread(buffer, 1, (size_t)size, readfile);

    fclose (readfile);

    *amount = CounterOfLines(buffer, size);

    SurgeryOfLines(buffer, size);

    struct Line *ptrstruct = CreateStruct(buffer, (size_t)size, *amount);

    return ptrstruct;
}
