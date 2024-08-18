CC = gcc
CFLAGS = -g
DEPS = preprocessor.h firstPass.h second_pass.h symbolTable.h opcodeTable.h memory_manager.h parsed_program.h parser.h tokenizer.h util.h
OBJ = main.o preprocessor.o firstPass.o second_pass.o symbolTable.o opcodeTable.o memory_manager.o parsed_program.o parser.o tokenizer.o util.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

assembler: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f *.o assembler