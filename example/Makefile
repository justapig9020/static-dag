SRC=$(wildcard *.c ../*.c)
OBJ=$(SRC:.c=.o)

main: $(OBJ)
	gcc -I.. -o main $(OBJ)

%.o: %.c
	gcc -I.. -c $< -o $@

clean:
	rm -f main $(OBJ)