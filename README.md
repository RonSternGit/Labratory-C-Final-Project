# Assembler in C – Final Project

This project is a simplified assembler written in C, developed as part of a systems programming lab course. It reads assembly language files, parses and validates the syntax, and generates machine code output.

## 🛠 Features

- First and second pass parsing
- Symbol table creation
- Error handling and reporting
- Output file generation (`.ob`, `.ent`, `.ext`)

## 📁 File Structure

- `main.c` – Entry point of the program
- `parser.c` / `parser.h` – Handles parsing logic
- `symbol_table.c` – Manages symbols and labels
- `utils.c` – Helper functions
- `assembler.c` – Core assembler logic

## 🚀 How to Run

```bash
gcc -Wall -o assembler *.c
./assembler input_file.as
