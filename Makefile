CC=gcc
CFLAGS=-fsanitize=address -g
SOURCES=firstPass.c tokenizer2.c opcodeTable.c symbolTable.c parser2.c util.c
TARGET=firstPass

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)