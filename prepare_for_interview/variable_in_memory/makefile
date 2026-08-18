# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Target executable
TARGET = program

# Source files (automatically detects all .cpp files)
SRCS = $(wildcard *.cpp)

# Header files (automatically detects all .h or .hpp files)
HEADERS = $(wildcard *.h) $(wildcard *.hpp)

# Object files (replaces .cpp with .o)
OBJS = $(SRCS:.cpp=.o)

# Default rule to build the executable
all: $(TARGET)

# Link object files to create the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) -pthread
	rm -f $(OBJS)

# Compile source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up compiled files
.PHONY: clean
clean:
	rm -f $(TARGET) $(OBJS)

# Run the program
.PHONY: run
run: $(TARGET)
	./$(TARGET)

