CFLAGS = -g -MD -c
GCOV_FLAGS = -fprofile-arcs -ftest-coverage

coverage: RB_Tree.gcno RB_Tree.gcda
	gcov -f RB_Tree.c && gcovr -f RB_Tree.c

test: unittests.out

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

check_code.out: unittests.c RB_Tree.c
	$(CC) $(GCOV_FLAGS) -o check_code.out $^

unittests.out: unittests.c RB_Tree.c
	$(CC) -o RB_tree.out $^

RB_Tree.gcno: unittests.c RB_Tree.c
	$(CC) $(GCOV_FLAGS) -o check_code.out $^

RB_Tree.gcda: check_code.out
	./check_code.out
-include *.d

.PHONY: clean

clean:
	rm *.o *.d *.gcda *.gcno *.gcov *.out *.dot *.jpg