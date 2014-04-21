CC = gcc
CFLAGS += -O3 -Wall
LDLIBS += -lncurses
PROG = guslife

OBJS = 	life.o \
		game.o \
		main.o


all: $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) $(LDLIBS)

.PHONY: clean

clean:
	-rm $(PROG) $(OBJS)

