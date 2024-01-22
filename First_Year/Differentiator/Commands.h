FUNCTION(sin, {NewBranch(MODE_OPERATOR, nullptr, '*', Diff(Node->Right), NewBranch(MODE_FUNCTION, "cos", POISON, NewBranch(NIL,
            nullptr, POISON, nullptr, nullptr), Copy(Node->Right)))})

FUNCTION(cos, {NewBranch(MODE_OPERATOR, nullptr, '*', Diff(Node->Right), NewBranch(MODE_OPERATOR, nullptr, '*',
NewBranch(MODE_FUNCTION, "sin", POISON, NewBranch(NIL, nullptr, POISON, nullptr, nullptr), Copy(Node->Right)),
NewBranch(MODE_NUMBER, nullptr, -1, nullptr, nullptr)))})

FUNCTION(tg, {NewBranch(MODE_OPERATOR, nullptr, '/', Diff(Node->Right),
NewBranch(MODE_OPERATOR, nullptr,'^', NewBranch(MODE_FUNCTION, "cos", POISON, nullptr, Copy(Node->Right)),
NewBranch(MODE_NUMBER, nullptr ,2, nullptr, nullptr)))})

FUNCTION(ctg, {NewBranch(MODE_OPERATOR, nullptr, '/', Diff(Node->Right),
NewBranch(MODE_OPERATOR, nullptr,'^', NewBranch(MODE_FUNCTION, "sin", POISON, nullptr, Copy(Node->Right)),
NewBranch(MODE_NUMBER, nullptr, 2, nullptr, nullptr)))})

FUNCTION(exp, {NewBranch(MODE_OPERATOR, nullptr, '*', NewBranch(MODE_FUNCTION, "exp", POISON, nullptr, Copy(Node->Right)), Diff(Node->Right))})

FUNCTION(ln, {NewBranch(MODE_OPERATOR, nullptr, '/', Diff(Node->Right), Copy(Node->Right))})

