# Assembler in C – Final Project

This project is a simplified assembler written in C, developed as part of the Systems Programming Lab course at the Open University of Israel. It reads assembly language files (`.as`), parses and validates the syntax, and generates machine code output files (`.ob`, `.ent`, `.ext`).

## 🛠 Features

- First and second pass parsing
- Symbol table creation
- Error handling and reporting
- Output file generation

## 📁 File Structure

All files are written in C (`.c`) and header files (`.h`):

- `main.c` – Entry point of the assembler
- `first_pass.c` / `first_pass.h` – Handles the first parsing pass
- `second_pass.c` / `second_pass.h` – Handles the second parsing pass
- `symbol_table.c` / `symbol_table.h` – Manages symbols and labels
- `utils.c` / `utils.h` – Helper functions and utilities
- `output.c` / `output.h` – Generates output files
- `parser.c` / `parser.h` – Parses assembly lines and instructions
- `errors.c` / `errors.h` – Error handling and reporting

## 🚀 How to Compile and Run

To compile the assembler:

```bash
gcc -Wall -o assembler *.c
```

To run the assembler on an input file:

```bash
./assembler example.as
```

Make sure to provide a valid `.as` file containing assembly code.

Example input files and sample outputs are available in the folders above.  
These include `.as` files with runnable assembly code and the corresponding generated output files (`.ob`, `.ent`, `.ext`) for reference.

## 📚 Technologies

C (compiled with GCC)  
Standard libraries only

## 🎓 Academic Context

This project was developed as part of the Systems Programming Lab course at the Open University of Israel. It demonstrates understanding of low-level programming, memory management, and multi-pass parsing techniques.

## 👤 Author

Ron Stern  
Computer Science student at the Open University of Israel
