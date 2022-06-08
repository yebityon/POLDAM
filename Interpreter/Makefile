DIR = include/Interpreter

test: methodEntry.o  main.o 
	g++-11 -o test main.o methodEntry.o

main.o: main.cpp
	g++-11 -I ./include -c main.cpp

methodEntry.o: $(DIR)/methodEntry.cpp $(DIR)/methodEntry.h
	g++-11 -I ./include -c $(DIR)/methodEntry.h
	g++-11 -I ./include -c $(DIR)/methodEntry.cpp

InterpreterCommon.o: $(DIR)/InterpreterCommon.h
	g++-11 -c $(DIR)/InterpreterCommon.h

clean:
	rm -f *.o
	rm -f *.out
