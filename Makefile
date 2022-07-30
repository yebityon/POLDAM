INT_DIR = Interpreter
INT_INCLUDE_DIR = $(INT_DIR)/include/Interpreter
INT_SRC_DIR = $(INT_DIR)/src
INT_TEST_DIR = $(INT_DIR)/test

METAFILE_DIR = MetafileParser
METAFILE_INCLUDE_DIR = MetafileParser/include
METAFILE_SRC_DIR = MetaFileParser/src

UTIL_DIR = Util
UTIL_INCLUDE_DIR = $(UTIL_DIR)/include/src

main: main.cpp util.o metafileParser.o 
	g++-11 -I./ main.cpp util.o metafileParser.o -o main

InterpreterTest: methodCallAndReturn.o util.o
	g++-11 $(INT_TEST_DIR)/main.cpp methodCallAndReturn.o util.o -o InterpreterTest
	./InterpreterTest
	rm  InterpreterTest

MetafileParserTest: metafileParser.o util.o
	g++-11 $(METAFILE_DIR)/test/main.cpp metafileParser.o util.o -o MetaFileParserTest
	./MetaFileParserTest
	rm MetaFileParserTest

methodCallAndReturn.o: $(INT_SRC_DIR)/methodEntry.cpp $(INT_SRC_DIR)/methodEntry.h $(INT_SRC_DIR)/methodParam.cpp $(INT_SRC_DIR)/methodParam.h $(INT_SRC_DIR)/methodNormalExit.cpp $(INT_SRC_DIR)/methodNormalExit.h util.o
	g++-11 -c $(INT_SRC_DIR)/*.cpp 
	ar rvs methodCallAndReturn.o methodEntry.o methodParam.o methodNormalExit.o

util.o: $(UTIL_INCLUDE_DIR)/poldamUtil.h $(UTIL_INCLUDE_DIR)/poldamUtil.cpp
	g++-11 -o util.o -c  $(UTIL_INCLUDE_DIR)/poldamUtil.cpp

metafileParser.o: $(METAFILE_SRC_DIR)/*.cpp
	g++-11 -c $(METAFILE_SRC_DIR)/*.cpp
	ar rvs metafileParser.o dataids.o factory.o seloggerLog.o

clean: *.o
	rm -f *.o
	rm -f *.out
