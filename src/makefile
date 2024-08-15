CC= gcc
CFLAGS= -g -fsanitize=address
SOURCES= main.c firstPass.c tokenizer.c opcodeTable.c symbolTable.c parser.c util.c parsed_program.c
TARGET= assembler

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)