INT_DIR = Interpreter
INT_INCLUDE_DIR = $(INT_DIR)/include/Interpreter
INT_SRC_DIR = $(INT_DIR)/src

UTIL_DIR = Util
UTIL_INCLUDE_DIR = $(UTIL_DIR)/include/src

test: methodEntry.o methodParam.o main.o util.o
	g++-11 -o test main.o methodEntry.o util.o

main.o: $(INT_SRC_DIR)/main.cpp
	g++-11 -I $(INT_DIR)/include -c $(INT_SRC_DIR)/main.cpp

methodEntry.o: $(INT_INCLUDE_DIR)/methodEntry.cpp $(INT_INCLUDE_DIR)/methodEntry.h util.o
	g++-11 -I$(INT_DIR)/include -I$(UTIL_INCLUDE_DIR) -c $(INT_INCLUDE_DIR)/methodEntry.cpp

methodParam.o: $(INT_INCLUDE_DIR)/methodParam.cpp $(INT_INCLUDE_DIR)/methodParam.h util.o
	g++-11 -I$(INT_DIR)/include -I$(UTIL_INCLUDE_DIR) -c $(INT_INCLUDE_DIR)/methodParam.cpp

util.o: $(UTIL_INCLUDE_DIR)/poldam_util.h $(UTIL_INCLUDE_DIR)/poldam_util.cpp
	g++-11 -o util.o -c  $(UTIL_INCLUDE_DIR)/poldam_util.cpp


clean:
	rm -f *.o
	rm -f *.out
