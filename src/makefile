CC= gcc 
CFLAGS= -Wall -ansi -pedantic
SOURCES= main.c secondPass.c preprocessor.c macro_table.c firstPass.c memory_manager.c tokenizer.c opcodeTable.c symbolTable.c parser.c util.c parsed_program.c
TARGET= assembler


$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)