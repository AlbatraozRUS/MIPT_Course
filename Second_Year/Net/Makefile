CFLAGS = -c -MD -g -Wall
THREADFLAG = -pthread
INCLUDES = -I./include -I./include/Network -I./include/Integration
SRC = main.c src/common.c src/Network/Worker.c src/Network/Server.c src/Integration/Calculation.c src/Integration/ThreadPlanner.c
OBJ = $(SRC:.c=.o)
EXEC = calc.out

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(THREADFLAG) $(INCLUDES) -o $@ $^

%.o: %.c
	$(CC) $(THREADFLAG) $(CFLAGS) $(INCLUDES) -o $@ $<

.PHONY: clean
clean:
	rm -rf $(OBJ) $(OBJ:.o=.d)

-include $(OBJ:.o=.d)
