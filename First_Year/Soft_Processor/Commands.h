#ifdef COMMANDS
CMD_COMPARE (Push, 16,
    {
    if (bytecode[PC + 1] == NUMBER) // arg is immediate constant
        {
        StackPush (&CPU->stk, *(int*) (bytecode + PC + 2));
        PC_MOVE ('i');
        break;
        }

    if (bytecode[PC + 1] == REGISTER) // arg is register
        {
        Dump (&CPU->stk);
        CPU->CR[bytecode[PC + 2] - 'a'] = StackPop(&CPU->stk);
        Dump (&CPU->stk);
        PC_MOVE ('c');
        }

    break;
    })

CMD_COMPARE (Pop, 17,
    {
    if (bytecode [PC + 1] == NOTHING)
    {
        StackPop(&CPU->stk);
        PC_MOVE('c');
        break;
    }
    if (bytecode [PC + 1] == REGISTER){
       StackPush(&CPU->stk, CPU->CR[bytecode[PC + 2] - 'a']);
       PC_MOVE('c');
       break;
       }
    })

CMD_COMPARE(Add, 18,
    {
    StackPush(&CPU->stk, StackPop(&CPU->stk) + StackPop(&CPU->stk));
    PC_MOVE('c');
    break;
    })

CMD_COMPARE(Sub, 19,
    {
    StackPush(&CPU->stk, -StackPop(&CPU->stk) + StackPop(&CPU->stk));
    PC_MOVE('c');
    break;
    })

CMD_COMPARE(Mul, 20,
    {
    StackPush(&CPU->stk, StackPop(&CPU->stk) * StackPop(&CPU->stk));
    PC_MOVE('c');
    break;
    })

CMD_COMPARE(Div, 21,
    {int temp1 = StackPop(&CPU->stk);
      int temp2 = StackPop(&CPU->stk);
    StackPush(&CPU->stk, temp2 / temp1);
    PC_MOVE('c');
    break;
    })

CMD_COMPARE(Sqrt, 22,
    {
    TempForDouble = StackPop(&CPU->stk);
    TempForDouble = sqrt(TempForDouble);
    TempForDouble = (int) TempForDouble;
    StackPush(&CPU->stk, TempForDouble);
    PC_MOVE('c');
    break;
    })

CMD_COMPARE(Sin, 23,
    {
    TempForDouble = StackPop(&CPU->stk);
    TempForDouble = sin(TempForDouble);
    TempForDouble = (int) TempForDouble;
    StackPush(&CPU->stk, TempForDouble);
    PC_MOVE('i');
    break;
    })

CMD_COMPARE(Cos, 24,
    {
    TempForDouble = StackPop(&CPU->stk);
    TempForDouble = cos(TempForDouble);
    TempForDouble = (int) TempForDouble;
    StackPush(&CPU->stk, TempForDouble);
    PC_MOVE('i');
    break;
    })

CMD_COMPARE(Out, 25,
    {
    printf("The answer is \"%d\"\n", StackPop(&CPU->stk));
    PC_MOVE('c');
    break;
    })

CMD_COMPARE(End, -1,
    {
    return;
    })

CMD_COMPARE(Jp, 60,
    {
    PC = *(int*) (bytecode + PC + 1);
    break;
    })

CMD_COMPARE(Ja, 65,
    {
    if (StackPop(&CPU->stk) > StackPop(&CPU->stk))
        PC = *(int*) (bytecode + PC + 1);
    else PC_MOVE('ij');
    break;
    })

CMD_COMPARE(Jea, 70,
    {
    if (StackPop(&CPU->stk) >= StackPop(&CPU->stk))
        PC = *(int*) (bytecode + PC + 1);
    else PC_MOVE('ij');
    break;
    })

CMD_COMPARE(Jb, 75,
    {
    if (StackPop(&CPU->stk) < StackPop(&CPU->stk))
        PC = *(int*) (bytecode + PC + 1);
    else PC_MOVE('ij');
    break;
    })

CMD_COMPARE(Jeb, 80,
    {
    if (StackPop(&CPU->stk) <= StackPop(&CPU->stk))
        PC = *(int*) (bytecode + PC + 1);
    else PC_MOVE('ij');
    break;
    })
CMD_COMPARE(Eq, 85,
{
    if (StackPop(&CPU->stk) == StackPop(&CPU->stk))
        PC = *(int*) (bytecode + PC + 1);
    else PC_MOVE('ij');
    break;
})

CMD_COMPARE(Call, 100,
{
    StackPush(&CPU->Addresses, PC + sizeof(int) + sizeof(char));
    PC = *(int*) (bytecode + PC + 1);
    break;
})

CMD_COMPARE(Ret, 110,
{
    PC = StackPop(&CPU->Addresses);
    break;
})

CMD_COMPARE(In, 26,
{   printf("Please input a number\n");
    scanf("%d", &TempForIn);
    StackPush(&CPU->stk, TempForIn);
    PC_MOVE('c');
    fflush (stdout);
    break;
})

CMD_COMPARE(CAT, 111,
{
    {
            	 	printf("                   /\\             /\\\n"
	                       "                  |`\\\\_,--=\"=--,_//`|  \n"
	                       "                  \\ .\"  :'. .':  \". /\n"
	                       "                 ==)  _ :  '  : _  (==\n"
	                       "                   |>/O\\   _   /O\\<|\n"
	                       "                   | \\-\"~` _ `~\"-/ |\n"
	                       "                  >|`===. \\_/ .===`|<\n"
	                       "        Ded .-\"-.   \\==='  |  '===/   .-\"-. By Iskander\n"
	                       "     .-----{'. '`}---\\,  .-'-.  ,/---{.'. '}-----.\n"
	                       "     |     `\"---\"`     `~-===-~`     `\"---\"`     |\n"
	                       "     |     .-------------------------------.     |\n"
	                       "     |     |                               |     |\n"
	                       "     |     |                               |     |\n"
	                       "     |     |                    .--.       |     |\n"
	                       "     |     |                  .\"  o \\__    |     |\n"
	                       "     |     |               _.-\"    ,(  `   |     |\n"
	                       "     |     |           _.-\"      ,;;|      |     |\n"
	                       "     |     |      _.-=\" _,\"    ,,;;;'      |     |\\  \n"
	                       "     |     |  .-\"`_.-\"``-..,,;;;;:'        |     |'\\\n"
	                       "     |     |  `\"'`          `\\`\\           |     |.'`\\\n"
	                       "     |     |                 /^\\\\\\         |     |'.'`}\n"
	                       "     |     |                               |     } '. }\n"
	                       "     |     |        Котики это круто!      |    /.'`./\n"
	                       "     |     '-------------------------------'   /. './\n"
	                       "    _|  _                                     /.`'./\n"
	                       "   (.\\-/.)--------.__________________.-------{.'. {\n"
	                       "    \\0 0/         /                  \\       `\\.'`\\\n"
	                       "(   =\\ /=  ______/....................\\______  \\.'.\\\n"
	                       " \\   /o\\  |                                  |  }'.'}\n"
	                       "  ) /   \\ | [[[[[[[[[[ ===       ()  ()  ()  | {'.'/\n"
	                       " (  U   U |                       ________   |  `~`\n"
	                       "  \\(|   |)| [[[[[[[[[[ ===       [________]  |\n"
	                       "   _w___w_:----------------------------------:_______\n"
	                       "  |  []  [][][][] [][][][] [][][][] [][][]  == ===== |\n"
	                       "  |                                                  |\n"
	                       "  |   [][][][][][][][][][][][][][]_ [][][] [][][][]  |\n"
	                       "  |   [_][][][][][][][][][][][][]| |[][][] [][][]||  |\n"
	                       "  |   []  [][][][][][][][][][][][__|       [][][]||  |\n"
	                       "  |   [__] [][][][][][][][][][][___]  []   [][][]||  |\n"
	                       "  |   [_]  [_][_____________][_] [_][][][] [__][]||  |\n"
	                       "  '--------------------------------------------------'\n");

                	PC_MOVE('c');
            	 }
})

#endif

#ifdef JUMPS

CMD_COMPARE(Jp, 60)

CMD_COMPARE(Ja, 65)

CMD_COMPARE(Jea, 70)

CMD_COMPARE(Jb, 75)

CMD_COMPARE(Jeb, 80)

CMD_COMPARE(Eq, 85)
#endif
