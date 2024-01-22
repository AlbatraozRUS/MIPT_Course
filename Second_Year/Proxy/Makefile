CC=clang
CFLAGS=-std=c11 -c

-all: Proxy.o
	$(CC) -o Proxy Proxy.o && rm Proxy.o lib.h.gch

Proxy.o:
	$(CC) $(CFLAGS) Proxy.c lib.h
