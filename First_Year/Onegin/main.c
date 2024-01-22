
//-----------------------------------------------------------
//! This program sorts the strings alphabetically in Hamlet
//-----------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>

#define MEOW

#ifdef MEOW
#define ASSERT(x)
#else
#define ASSERT(x) assert(x)
#endif

const char * HAMLETINPUT  = "WAY TO YOUR FILE FOR INPUT";
const char * HAMLETOUTPUT = "WAY TO YOUR FILE FOR OUTPUT";

const int Error_Way_File = -100;

struct Line {
    char *ptrbuffer;
    unsigned long length;
};


//Functions, which compares two structs
//!@param[in] ptr1 Pointer to the first struct
//!@param[in] ptr2 Pointer to the second struct
//!@return -1, 0, 1 According to seniority of strings

//From the beginning
int Comparator_Begining (const void* p1, const void* p2)
{
    assert(p1 != NULL);
    assert(p2 != NULL);

    struct Line str1 = *(struct Line const*) p1;
    struct Line str2 = *(struct Line const*) p2;

    char *string1 = str1.ptrbuffer;
    char *string2 = str2.ptrbuffer;

    for (; *string1 || *string2; *string1++, *string2++) {

    if (!isalpha(*string1)) {
            string2--;
            continue;
        }
        if  (!isalpha(*string2)) {
            string1--;
            continue;
        }
        if (*string1 == *string2)
            continue;

        if (*string2 > *string1) {
           return -1;
        }
        if (*string2 < *string1) {
            return 1;
        }
    }

    return 0;
}

//From the ending
int Comparator_Ending (const void *p1, const void *p2)
{
    assert(p1 != NULL);
    assert(p2 != NULL);

    struct Line str1 = *(struct Line const*) p1;
    struct Line str2 = *(struct Line const*) p2;

    char *string1 = str1.ptrbuffer + str1.length - 2;
    char *string2 = str2.ptrbuffer + str2.length - 2;

    for (; *string1 || *string2; string1--, string2--) {

        if (!isalpha(*string1)) {
            string2++;
            continue;
        }
        if  (!isalpha(*string2)){
            string1++;
        }

        if (*string1 == *string2) {
            continue;
        }

        if (*string1 > *string2) {
            return -1;
        }

        if (*string1 < *string2) {
            return 1;
        }
    }
    return 0;
}

//Function counts the amount of symbols
//!@param[in] readfile The file
//!@return The amount of symbols

size_t CounterOfSymbols(FILE * readfile)
{

    assert(readfile != NULL);

    fseek(readfile , 0 , SEEK_END);
    size_t size = ftell(readfile);
    fseek(readfile , 0 , SEEK_SET );

    return size;
}

//Function changes \n to \0
//!@param[in] buffer buffer with text
//!@param[in] size the number of symbols

void SurgeryOfLines(char *buffer, long size){

    for (int i = 0; i < size; i++){
        if (buffer[i] == '\n'){
            buffer[i] = '\0';
        }
    }
}

//Function counts the number of lines
//!@param[in] buffer The buffer with the text
//!@param[in] size The amount of symbols in the text
//!@return The amount of lines

size_t CounterOfLines(char *buffer, long size)
{

    assert(buffer != NULL);

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

//Function fulls struct with lengths and pointers
//!param[in] buffer The buffer with the text
//!param[in] size The amount of symbols
//!param[in] amount The amoubt of lines
//!@return Created structure

struct Line* CreateStruct(char *buffer, size_t size, size_t amount)
{
    assert(buffer != NULL);

    struct Line *ptrstruct = (struct Line*) calloc(amount, sizeof(ptrstruct[0]));
    assert(ptrstruct != NULL);

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


// Function reads the file
//! @param[in] amount Number of strings
//!@param[in] buffer Array with text before reading
//!@param[in] size Number of symbols
//!@param[in] indicator Indicates the code error
//!@return buffer Array with text after reading
//! Note: Error means that there is
//! something wrong with way to the file

char * Reading(char *buffer, size_t *amount, int * indicator, size_t* size)
{

    assert(amount != NULL);

    FILE *readfile = fopen( HAMLETINPUT , "r" );

    if (readfile == NULL)
        *indicator = Error_Way_File;

    *size = CounterOfSymbols(readfile);

    buffer = calloc(*size + 1, sizeof(char));
    assert(buffer != NULL);

    fread(buffer, 1, *size, readfile);

    fclose (readfile);

    *amount = CounterOfLines(buffer, *size);

    SurgeryOfLines(buffer, *size);


    return buffer;
}

//Function writes down into file using array of pointers
//! @param[in] index - The number of strings
//! @param[in] ptrbuffer Array of pointers to the text

void Writing (size_t amount, FILE *writefile, int *indicator, struct Line *ptrstruct)
{

    assert(writefile != NULL);

    if (writefile == NULL)
        *indicator = Error_Way_File;

    for (int i = 0; i < amount; i++) {
        fprintf(writefile, "%s\n", (*(ptrstruct + i)).ptrbuffer);
    }

    fprintf(writefile, "\n\n\n--------------------------------------------------\n\n\n");

}

//Main function
//! @file main.c
//! @mainpage
//! - main.c
//Note: If program finishes with code -100 check your way to the file

int main()
{

    printf("\nHello! This program can sort alphabetically your text\n");

    char *buffer  = NULL;
    size_t amount = 0;
    size_t size = 0;
    int indicator = 0;

    //Считывание
    buffer = Reading(buffer, &amount, &indicator, &size);
    struct Line * ptrstruct = CreateStruct(buffer, size, amount);
    if (indicator == Error_Way_File)
        return Error_Way_File;
    printf("\nFile was read\n\n");

    //Сортировка с начала
    qsort(ptrstruct, amount, sizeof(ptrstruct[0]) , Comparator_Begining);
    printf("File was sorted first time\n\n");

    //Вывод с начала
    FILE *writefile = fopen(HAMLETOUTPUT, "w");
    Writing( amount, writefile, &indicator, ptrstruct);
    if(indicator == Error_Way_File)
        return Error_Way_File;

    //Сортировка с конца
    qsort(ptrstruct, amount, sizeof(ptrstruct[0]), Comparator_Ending);
    printf("File was sorted second time\n\n");

    //Вывод с конца
    Writing(amount, writefile, &indicator, ptrstruct);

    //Очистка и закрытие
    fclose(writefile);
    free(buffer);
    free(ptrstruct);

    printf("The task is completed\n");

    return 0;
}
