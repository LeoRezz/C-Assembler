CC= gcc
CFLAGS= -Wall -ansi -pedantic -g -fsanitize=address
SOURCES= firstPass.c tokenizer.c opcodeTable.c symbolTable.c parser.c util.c
TARGET= firstPass

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)