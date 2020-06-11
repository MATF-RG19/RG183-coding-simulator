PROGRAM   = AnotherHourOfCode
CC        = gcc
CFLAGS    = -g -Wall
DEPS      = figure.h carpet.h image.h
LDFLAGS   = -lGL -lGLU -lglut -lm

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(PROGRAM): main.o figure.o floor_setup.o image.o
	$(CC) -o $(PROGRAM) main.o figure.o floor_setup.o image.o $(LDFLAGS)

.PHONY: clean dist run

clean:
	-rm -f *.o $(PROGRAM) *core

dist: clean
	-tar -chvj -C .. -f ../$(PROGRAM).tar.bz2 $(PROGRAM)

run:
	./$(PROGRAM)

