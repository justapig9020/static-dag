SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)

main: $(OBJ)
	gcc -o main $(OBJ)

%.o: %.c
	gcc -c $<

clean:
	rm -f main $(OBJ)