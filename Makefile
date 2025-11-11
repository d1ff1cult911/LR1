CXX = g++
CXXFLAGS = -std=c++17 -Iinclude
SRC = src/Lazy_sequence.cpp src/Tests.cpp main.cpp
OUT = LR1.exe

all:
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: all
	.\$(OUT)

clean:
	del $(OUT)
