sources = main.c bits.c
objects = $(patsubst %.c,%.o,$(sources))
CC = gcc
OPTS = -Wall
PROG = iHevc

$(PROG): $(objects)
	$(CC) $(OPTS) -o $@ $(objects)

main.o: main.c
	$(CC) -c $<

bits.o: bits.c
	$(CC) $(OPTS) -c $<

clean:
	$(RM) $(PROG) $(objects)

