CC     = gcc
CFLAGS = -Wall -Wextra
LIBS   =

PROGRAM = cat
SRCS    = $(wildcard *.c)
OBJS    = $(SRCS:.c=.o)

$(PROGRAM): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROGRAM) $(OBJS) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(PROGRAM) $(OBJS)

.PHONY: clean
