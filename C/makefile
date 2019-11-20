CC = g++

CFLAGS = -Wall -g
INCLUDES = -I./

LFLAGS = -Liniparser
LIBS = -liniparser

SRCS = main.c lorenz.c dummy.c
OBJS = $(SRCS:.c=.o)

MAIN = main

.PHONY: depend clean

all: $(MAIN)

$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

