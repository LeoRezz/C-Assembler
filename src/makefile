CC= gcc
CFLAGS= -g -fsanitize=address -lm
SOURCES= main.c firstPass.c memory_manager.c tokenizer.c opcodeTable.c symbolTable.c parser.c util.c parsed_program.c
TARGET= assembler
PREPROCESSOR_SOURCE= preprocessor.c util.c
PREPROCESSOR_TARGET= preprocessor
SECOND_PASS_SOURCE= secondPass.c util.c parsed_program.c memory_manager.c opcodeTable.c symbolTable.c
SECOND_PASS_TARGET= secondPass
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

prep: $(PREPROCESSOR_SOURCE)
	$(CC) $(CFLAGS) $(PREPROCESSOR_SOURCE) -o $(PREPROCESSOR_TARGET)

secondPass: $(SECOND_PASS_SOURCE)
	$(CC) $(CFLAGS) $(SECOND_PASS_SOURCE) -o $(SECOND_PASS_TARGET)

clean:
	rm -f $(TARGET) $(PREPROCESSOR_TARGET)