# Compiler

This is a recursive descent compiler built for a systems software course.
It takes as input a .mc file and outputs 64 bit intel assembly representing it. 
Capabilities include if-else statements, while loops, and multiple functions.
Main components are the lexer, parser, and symbol table. Requires nasm and gcc.

# How to use

To make the microc executable simply type make on the command line. Run the 
executable on the included example .mc file and redirect its output to a .asm
file with 'microc example.mc > example.asm'. Make an object file using nasm
with 'nasm -f elf64 example.asm'. Finally make an executable with 'gcc -o example example.o'.
This will generate an example executable which you can run with './example'.