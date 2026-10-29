CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wconversion -Wextra -Wpedantic -g
TARGET = main
SRC = src/main.cpp

all: $(TARGET)

$(TARGET): $(SRC) src/Set.h
  $(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
