CC = gcc
CFLAGS = -std=gnu11 -Wall -Wextra -Wno-unused-parameter

all: btest

btest: btest.c bits.c decl.c tests.c btest.h bits.h
	$(CC) $(CFLAGS) -o btest bits.c btest.c decl.c tests.c

# Validate the tester, reference functions, signatures, domains, and scoring.
selftest: btest
	./btest -S -g

# Score the current student submission.
grade: btest
	./btest -g

ifeq ($(OS),Windows_NT)
clean:
	-del /Q *.o btest btest.exe 2>NUL
else
clean:
	rm -f *.o btest btest.exe
endif

.PHONY: all selftest grade clean

