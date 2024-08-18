CC = gcc
CFLAGS = -g

tokenizer_test: tokenizer_test.c ../src/tokenizer.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f tokenizer_test