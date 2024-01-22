#include "Hash_Table.h"

unsigned int NotReallyHash()
{
    return 1;
}

unsigned int FirstLetter(const char *Line)
{
    return (unsigned int) Line[0];
}

unsigned int Length(const Line *String, const int NumLine)
{
    unsigned int Length = 0;
    for (int Letter = 0;String[NumLine].ptrbuffer[Letter]; Letter++)
        Length++;
    
    return Length;
}

unsigned int SumOfLetters (const Line *String, const int NumLine)
{
    int Sum = 0;
    for (int Letter = 0; Letter < String[NumLine].length; Letter++)
        Sum += (int)String[NumLine].ptrbuffer[Letter];
    return (unsigned int) Sum;
}

int Roll(const char* String)
{
    if(!(*String))
        return 0;

    char temp = *String;
    char c = '\0';

    c = c | *String;
    c = c >> 7;

    temp = temp << 1;
    temp = temp | c;

    return temp + Roll(String + 1);
}

int GNUHash(const char* string, char stopper)
{

    int h = 5381;

    for (char c = *string; c != stopper; c = *++string)
        h = h * 33 + c;

    h %= 1000;

    return (h < 0)? -h : h;
}

unsigned int MurmurHash (char * key, unsigned int len)
{
    const unsigned int m = 0x5bd1e995;
    const unsigned int seed = 0;
    const int r = 24;

    unsigned int h = seed ^ len;

    const unsigned char * data = (const unsigned char *)key;
    unsigned int k;

    while (len >= 4)
    {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
    }

    switch (len)
    {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
    };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
}
