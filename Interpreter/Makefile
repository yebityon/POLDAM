DIR = include/Interpreter

Interpreter: methodEntry.o InterpreterCommon.o main.o 
	g++-11 -o Interpreter.out main.o InterpreterCommon.o methodEntry.o

test: main.cpp
	g++-11 -o main.out  -I ./include main.cpp

main.o: main.cpp
	g++-11 -I ./include -c main.cpp

methodEntry.o: methodEntry.o InterpreterCommon.o
	g++-11 -I ./include -c $(DIR)/methodEntry.h

InterpreterCommon.o: InterpreterCommon.o
	g++-11 -c $(DIR)/InterpreterCommon.h