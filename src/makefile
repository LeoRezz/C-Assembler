CC= gcc
CFLAGS= -g -fsanitize=address
SOURCES= main.c firstPass.c tokenizer.c opcodeTable.c symbolTable.c parser.c util.c parsed_program.c
TARGET= assembler
PREPROCESSOR_SOURCE= preprocessor.c util.c
PREPROCESSOR_TARGET= preprocessor

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

prep: $(PREPROCESSOR_SOURCE)
	$(CC) $(CFLAGS) $(PREPROCESSOR_SOURCE) -o $(PREPROCESSOR_TARGET)

clean:
	rm -f $(TARGET) $(PREPROCESSOR_TARGET)