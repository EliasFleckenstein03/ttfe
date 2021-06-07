CC=cc

FLAGS=-g -Wall -pedantic -Wextra -Wconversion\
	-Waggregate-return -Wcast-align -Wcast-qual -Wchar-subscripts\
	-Wdisabled-optimization -Wimplicit -Wimport -Winline\
	-Winit-self -Wlong-long -Wmissing-braces\
	-Wmissing-field-initializers -Wparentheses -Wunreachable-code\
	-Wunused -Wunused-value -Wunused-variable -Wwrite-strings

OBJS=$(SOURCES:.c=.o)
SOURCES=$(wildcard *.c)
EXEC=ttfe

all: $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(EXEC)

%.o: %.c
	$(CC) -c $(FLAGS) $< -o $@


clean:
	rm -rf *.o
	rm $(EXEC)
