CC=gcc
CFLAGS=-lm -lTableauNoir -Wall -g
LDFLAGS=
EXEC=demineur
SRC=$(wildcard *.c)

#
#Règles implicites
#
.SUFFIXES: .c
.c.o:
	$(CC) -c $< $(CFLAGS)

$(EXEC): $(patsubst %.c,%.o,$(SRC))
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -Rf *.o $(EXEC)

run: $(EXEC)
	./$(EXEC)

debug: $(EXEC)
	gdb -tui ./$(EXEC)
