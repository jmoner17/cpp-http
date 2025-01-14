# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17

# Targets and dependencies
TARGET = program
SOURCES = main.cc socketProperties.cc
HEADERS = socketProperties.h
OBJECTS = $(SOURCES:.cc=.o)

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files into object files
%.o: %.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)
