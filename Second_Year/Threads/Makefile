all: Threads.out Launch.sh

Threads.out: Threads.c
		g++ -pthread -o $@ $<

Launch.sh:
		chmod +x Launch.sh
