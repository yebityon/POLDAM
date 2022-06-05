DIR = include/Interpreter

Interpreter: main.o methodEntry.o LogEventType.o
	g++-11 -o Interpreter.out main.o methodEntry.o

main.o: main.cpp
	g++-11 -c main.cpp

methodEntry.o: methodEntry.o
	g++-11 -c $(DIR)/methodEntry.h

LogEventType.o: LogEventType.o
	g++-11 -c $(DIR)/LogEventType.h