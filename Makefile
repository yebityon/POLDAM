INT_DIR = Interpreter
INT_INCLUDE_DIR = $(INT_DIR)/include/Interpreter
INT_SRC_DIR = $(INT_DIR)/src
METAFILE_DIR = MetafileParser
METAFILE_INCLUDE_DIR = MetafileParser/include
METAFILE_SRC_DIR = MetaFileParser/src
UTIL_DIR = Util
UTIL_INCLUDE_DIR = $(UTIL_DIR)/include/src

main: main.cpp util.o metafileParser.o 
	g++-11 -I./ main.cpp util.o metafileParser.o -o main

InterpreterTest: methodEntry.o methodParam.o InterpreterTest.o methodExit.o util.o
	g++-11 -o InterpreterTest methodEntry.o methodParam.o methodNormalExit.o InterpreterTest.o util.o
	./InterpreterTest

MetafileParserTest: metafileParser.o util.o
	g++-11 -o MetafileParserTest -I$(UTIL_INCLUDE_DIR) metafileParser.o util.o
	./MetaFileParser
	
InterpreterTest.o: $(INT_SRC_DIR)/main.cpp
	g++-11 -o InterpreterTest.o -I $(INT_DIR)/include -c $(INT_SRC_DIR)/main.cpp

methodEntry.o: $(INT_INCLUDE_DIR)/methodEntry.cpp $(INT_INCLUDE_DIR)/methodEntry.h util.o
	g++-11 -I $(INT_DIR)/include -I$(UTIL_INCLUDE_DIR) -c $(INT_INCLUDE_DIR)/methodEntry.cpp

methodParam.o: $(INT_INCLUDE_DIR)/methodParam.cpp $(INT_INCLUDE_DIR)/methodParam.h util.o
	g++-11 -I $(INT_DIR)/include -I$(UTIL_INCLUDE_DIR) -c $(INT_INCLUDE_DIR)/methodParam.cpp

methodExit.o: $(INT_INCLUDE_DIR)/methodNormalExit.cpp $(INT_INCLUDE_DIR)/methodNormalExit.h util.o
	g++-11 -I $(INT_DIR)/include -I$(UTIL_INCLUDE_DIR) -c $(INT_INCLUDE_DIR)/methodNormalExit.cpp

util.o: $(UTIL_INCLUDE_DIR)/poldamUtil.h $(UTIL_INCLUDE_DIR)/poldamUtil.cpp
	g++-11 -o util.o -c  $(UTIL_INCLUDE_DIR)/poldamUtil.cpp

metafileParser.o: MetafileParser/include/factory.cpp MetafileParser/include/dataids.cpp 
	g++-11 -c $(METAFILE_INCLUDE_DIR)/dataids.cpp $(METAFILE_INCLUDE_DIR)/factory.cpp -I$(METAFILE_INCLUDE_DIR) -I./
	ar rvs metafileParser.o dataids.o factory.o

clean: *.o
	rm -f test
	rm -f *.o
	rm -f *.out
