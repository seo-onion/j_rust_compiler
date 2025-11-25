# Makefile para compilador de Rust en C++

CXX = g++
CXXFLAGS = -std=c++17 -Wall
TARGET = compiler
SOURCES = main.cpp ast.cpp token.cpp scanner.cpp parser.cpp visitor.cpp ast_to_dot.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f *.s *.dot *.png *.tokens.txt
	rm -rf backend/temp_compilations

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
