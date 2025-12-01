CXX = g++
CXXFLAGS = -std=c++17 -Iinclude

SRC_MAIN = main.cpp src/LazyMultiTuringMachine.cpp
SRC_TEST = src/tests.cpp src/LazyMultiTuringMachine.cpp

all:
	$(CXX) $(CXXFLAGS) $(SRC_MAIN) -o multitm.exe

tests:
	$(CXX) $(CXXFLAGS) $(SRC_TEST) -o tests.exe

clean:
	del *.exe
