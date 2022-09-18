METAFILE_DIR = MetafileParser
METAFILE_INCLUDE_DIR = MetafileParser/include
METAFILE_SRC_DIR = MetaFileParser/src

GRAPH_DIR = Graph
GRAPH_SRC_DIR = $(GRAPH_DIR)/src

UTIL_DIR = Util
UTIL_INCLUDE_DIR = $(UTIL_DIR)/include/src

BOOST_PATH = usr/local/boost_1_80_0

main: main.cpp util.o metafileParser.o graph.o
	g++-11 -I/$(BOOST_PATH) -I./ main.cpp util.o metafileParser.o graph.o -o main

MetafileParserTest: metafileParser.o util.o
	g++-11 $(METAFILE_DIR)/test/main.cpp metafileParser.o util.o -o MetaFileParserTest
	./MetaFileParserTest
	rm MetaFileParserTest

util.o: $(UTIL_INCLUDE_DIR)/poldamUtil.h $(UTIL_INCLUDE_DIR)/poldamUtil.cpp
	g++-11 -o util.o -c  $(UTIL_INCLUDE_DIR)/poldamUtil.cpp

metafileParser.o: $(METAFILE_SRC_DIR)/*.cpp $(METAFILE_SRC_DIR)/*.h
	g++-11 -c $(METAFILE_SRC_DIR)/*.cpp
	ar rvs metafileParser.o dataids.o factory.o objectFile.o seloggerLog.o methods.o classes.o
	rm factory.o
	rm objectFile.o
	rm seloggerLog.o
	rm dataids.o
	rm methods.o
	rm classes.o 

graph.o: $(GRAPH_SRC_DIR)/*.cpp $(GRAPH_SRC_DIR)/*.h
	g++-11 -I/$(BOOST_PATH) -c $(GRAPH_SRC_DIR)/*.cpp

clean: *.o
	rm -f *.o
	rm -f *.out
