# Compilation macros
 CC = gcc
 CFLAGS = -Wall -ansi -pedantic -g # Flags
 GLOBAL_DEPS = general_header.h # Dependencies for everything
 EXE_DEPS = assembler.o  auxiliary_functions.o table.o pre_assembler.o first_pass.o second_pass.o convertor.o parser.o errors.o # Deps for exe

 ## Executable
assembler: $(EXE_DEPS) $(GLOBAL_DEPS)
	$(CC) -g $(EXE_DEPS) $(CFLAGS) -o $@

assembler.o:  assembler.c $(GLOBAL_DEPS)
	$(CC) -c assembler.c $(CFLAGS) -o $@

pre_assembler.o: pre_assembler.c pre_assembler.h $(GLOBAL_DEPS)
	$(CC) -c pre_assembler.c $(CFLAGS) -o $@

first_pass.o: first_pass.c first_pass.h $(GLOBAL_DEPS)
	$(CC) -c first_pass.c $(CFLAGS) -o $@

second_pass.o: second_pass.c second_pass.h $(GLOBAL_DEPS)
	$(CC) -c second_pass.c $(CFLAGS) -o $@

convertor.o: convertor.c convertor.h $(GLOBAL_DEPS)
	$(CC) -c convertor.c $(CFLAGS) -o $@

parser.o: parser.c parser.h $(GLOBAL_DEPS)
	$(CC) -c parser.c $(CFLAGS) -o $@

table.o: table.c table.h $(GLOBAL_DEPS)
	$(CC) -c table.c $(CFLAGS) -o $@

auxiliary_functions.o: auxiliary_functions.c auxiliary_functions.h $(GLOBAL_DEPS)
	$(CC) -c auxiliary_functions.c $(CFLAGS) -o $@

errors.o: errors.c errors.h $(GLOBAL_DEPS)
	$(CC) -c errors.c $(CFLAGS) -o $@

clean:
	rm -rf *.o *.am *.ob *.ent *.ext
