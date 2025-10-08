# Assembler in C – Final Project

This project is a simplified assembler written in C, developed as part of a systems programming lab course. It reads assembly language files, parses and validates the syntax, and generates machine code output.

## 🛠 Features

- First and second pass parsing
- Symbol table creation
- Error handling and reporting
- Output file generation (`.ob`, `.ent`, `.ext`)

## 📁 File Structure

- `main.c` – Entry point of the assembler
- `first_pass.c` / `first_pass.h` – Handles the first parsing pass
- `second_pass.c` / `second_pass.h` – Handles the second parsing pass
- `symbol_table.c` / `symbol_table.h` – Manages symbols and labels
- `utils.c` / `utils.h` – Helper functions and utilities
- `output.c` / `output.h` – Generates output files (.ob, .ent, .ext)
- `parser.c` / `parser.h` – Parses assembly lines and instructions
- `errors.c` / `errors.h` – Error handling and reporting

## 🚀 How to Run

```bash
gcc -Wall -o assembler *.c
./assembler input_file.as
