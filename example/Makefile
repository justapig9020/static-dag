SRC=$(wildcard *.c ../*.c)
OBJ=$(SRC:.c=.o)

main: $(OBJ)
	gcc -I.. -o main $(OBJ)

%.o: %.c
	gcc -I.. -c $< -o $@

run: main
	./main 2> test.md

clean:
	rm -f main $(OBJ)
