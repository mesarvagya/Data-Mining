CXX = g++
DEBUG = -g
CXXFLAGS = -Wall $(DEBUG) -std=c++11

SRCS = main.cpp sp0090_bns0028ARFFParser.cpp sp0090_bns0028ARFFWriter.cpp sp0090_bns0028NaiveBayes.cpp
OBJS = $(subst .cpp,.o,$(SRCS))

all: sp0090_bns0028NaiveBayes

sp0090_bns0028NaiveBayes: $(OBJS)
	$(CXX) $(CXXFLAGS) -o sp0090_bns0028NaiveBayes $(OBJS)

main.o: main.cpp sp0090_bns0028ARFFParser.hpp sp0090_bns0028ARFFWriter.hpp sp0090_bns0028NaiveBayes.hpp globals.hpp
	$(CXX) $(CXXFLAGS) -c main.cpp

ArffParser.o: sp0090_bns0028ARFFParser.hpp globals.hpp
	$(CXX) $(CXXFLAGS) -c sp0090_bns0028ARFFParser.cpp

ArffWriter.o: sp0090_bns0028ARFFWriter.hpp globals.hpp
	$(CXX) $(CXXFLAGS) -c sp0090_bns0028ARFFWriter.cpp

NaiveBayes.o: sp0090_bns0028NaiveBayes.hpp globals.hpp
	$(CXX) $(CXXFLAGS) -c sp0090_bns0028NaiveBayes.cpp

.PHONY: clean

clean:
	rm -f sp0090_bns0028NaiveBayes *.o *.gch


