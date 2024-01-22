#include "Differentiator.h"

const char *Symbol = "";

Branch* GetE();
Branch* GetT();
Branch* GetPow();
Branch* GetP();
Branch* GetN();
Branch* GetF();

Branch* GetG (const char *Expression)
{
   Symbol = Expression;

   Branch *Value = GetE();

   return Value;
}

Branch* GetE()
{
    Branch *Value1 = GetT();
    Branch *Value3 = Value1;
    while (*Symbol == '+' || *Symbol == '-')
    {
        char Operator = *Symbol;
        Symbol++;
        Branch *Value2 = GetT();
        if (Operator == '+')
            Value3 = NewBranch(MODE_OPERATOR, nullptr, '+', Value1, Value2);
        else
            Value3 = NewBranch(MODE_OPERATOR, nullptr, '-', Value1, Value2);
        Value1->Parent = Value3;
        Value2->Parent = Value3;
    }

    return Value3;
}

Branch* GetT()
{
   Branch *Value1 = GetPow();
   Branch *Value3 = Value1;
   while (*Symbol == '*' || *Symbol == '/')
    {
        char Operator = *Symbol;
        Symbol++;
        Branch *Value2 = GetPow();
        if (Operator == '*')
            Value3 = NewBranch(MODE_OPERATOR, nullptr, '*', Value1, Value2);
        else
            Value3 = NewBranch(MODE_OPERATOR, nullptr, '/', Value1, Value2);
        Symbol++;
    }

    return Value3;
}

Branch *GetPow()
{
    Branch *Value1 = GetP();
    if (*Symbol == '^'){
        Symbol++;
        Branch *Value2 = NewBranch(MODE_OPERATOR, nullptr, '^', Value1, GetP());

        Value1->Parent = Value2;
        Value2->Right->Parent = Value2;

        return Value2;
    }

    return Value1;
}

Branch* GetP ()
{
    Branch* Value = nullptr;
    if (*Symbol == '('){
        Symbol++;
        Value = GetE();
        Symbol++;
        return Value;
    }

    if (isalpha(*Symbol) != 0){
        if (isalpha(*(Symbol + 1)) == 0){
            Value = GetN();
            return Value;
        }
        else{
            Branch* Value2 = GetF();
            Value = GetP();
            Value2->Right = Value;
            Value->Parent = Value2;
            
            return Value2;
        }
    }
    return GetN();
}

Branch* GetN()
{
    int Value = 0;
    while (*Symbol >= '0' && *Symbol <= '9'){
        Value = Value * 10 + *Symbol - '0';
        Symbol++;
    }
    if (isalpha(*Symbol) != 0){
        Branch* Value2 = NewBranch(MODE_VARIABLE, nullptr, *Symbol, nullptr, nullptr);
        Symbol++;
        return Value2;
    }

    return NewBranch(MODE_NUMBER, nullptr, Value, nullptr, nullptr);
}

Branch* GetF()
{
    char Name[10] = "";

//    for (int index = 0; isalpha(*Symbol) != 0; Symbol++, index++)
//        Name[index] = *Symbol;
    unsigned ScannedLetters = 0;
    sscanf(Symbol, "%[a-z]%n", Name, &ScannedLetters);
    Symbol += ScannedLetters;

    Branch *Value  = (Branch*) calloc(1, sizeof(Branch));
    Data *Info     = (Data*)   calloc(1, sizeof(Data));

    Value->Elem    = Info;
    Info->Type     = MODE_FUNCTION;
    Info->Function = strdup(Name);

    return Value;
}
