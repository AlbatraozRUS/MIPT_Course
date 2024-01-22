
#include "Processor.h"

size_t CounterOfSymbols(FILE * readfile)
{

    assert(readfile != nullptr);

    fseek(readfile , 0 , SEEK_END);
    size_t size =  ftell(readfile);
    fseek(readfile , 0 , SEEK_SET );

    return size;
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

char * Reading(size_t *amount)
{

    assert(amount != nullptr);

    FILE *readfile = fopen( Input_File , "rb" );

    size_t size = CounterOfSymbols(readfile);
    char *buffer = (char*) calloc(size, sizeof(char));
    assert(buffer != nullptr);

    fread(buffer, 1, size, readfile);

    fclose (readfile);

    *amount = CounterOfLines(buffer, size);

    return buffer;
}
