CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic -I../src

tokenizer_test: tokenizer_test.c ../src/tokenizer.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f tokenizer_test