CXX = g++
DEBUG = -g
CXXFLAGS = -Wall $(DEBUG) -std=c++11

SRCS = main.cpp sp0090_bns0028ARFFParser.cpp sp0090_bns0028ARRFWriter.cpp sp0090_bns0028DecisionTree.cpp
OBJS = $(subst .cpp,.o,$(SRCS))

all: sp0090_bns0028DecitionTree

sp0090_bns0028DecitionTree: $(OBJS)
	$(CXX) $(CXXFLAGS) -o sp0090_bns0028DecitionTree $(OBJS)

main.o: main.cpp sp0090_bns0028ARFFParser.hpp sp0090_bns0028ARRFWriter.hpp sp0090_bns0028DecisionTree.h globals.h
	$(CXX) $(CXXFLAGS) -c main.cpp

ArffParser.o: sp0090_bns0028ARFFParser.hpp globals.h
	$(CXX) $(CXXFLAGS) -c sp0090_bns0028ARFFParser.cpp

ArffWriter.o: sp0090_bns0028ARRFWriter.hpp globals.h
	$(CXX) $(CXXFLAGS) -c sp0090_bns0028ARRFWriter.cpp

DecisionTree.o: sp0090_bns0028DecisionTree.h globals.h
	$(CXX) $(CXXFLAGS) -c sp0090_bns0028DecisionTree.cpp

.PHONY: clean

clean:
	rm -f sp0090_bns0028DecitionTree *.o *.gch

