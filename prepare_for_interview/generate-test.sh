
function generate-test-without-header-func()
{
  # touch ./$1/$1.cpp
  echo "#include <iostream>

class $1
{
private:
    /* data */
public:
    $1(/* args */){};
    ~$1(){};
    void print_$1(){std::cout << \"$1 \"<< std::endl;};

};

int main()
{
    $1 t;
    t.print_$1();
    return 0;
}" > ./$1/$1.cpp
#makefile
  echo "# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++17

# Target executable
TARGET = program

# Source files (automatically detects all .cpp files)
SRCS = \$(wildcard *.cpp)

# Header files (automatically detects all .h or .hpp files)
HEADERS = \$(wildcard *.h) \$(wildcard *.hpp)

# Object files (replaces .cpp with .o)
OBJS = \$(SRCS:.cpp=.o)

# Default rule to build the executable
all: \$(TARGET)

# Link object files to create the final executable
\$(TARGET): \$(OBJS)
	\$(CXX) \$(CXXFLAGS) -o \$(TARGET) \$(OBJS) -pthread
	rm -f \$(OBJS)

# Compile source files into object files
%.o: %.cpp \$(HEADERS)
	\$(CXX) \$(CXXFLAGS) -c \$< -o \$@

# Clean up compiled files
.PHONY: clean
clean:
	rm -f \$(TARGET) \$(OBJS)

# Run the program
.PHONY: run
run: \$(TARGET)
	./\$(TARGET)
" > ./$1/makefile
}
if [ -z "$2" ]; then

  if [ -z "$1" ]; then
    echo "Please provide a name for the test directory"
    exit 1
  fi
  if [ -d ./$1 ]; then
    echo "Directory ./$1 already exists"
    exit 1
  fi
  mkdir ./$1
  generate-test-without-header-func $1
  code ./ &

else
  if [ "$2" == "delete" -o "$2" == "remove" ]; then
    if [ -d ./$1 ]; then
      rm -rf ./$1
    else
      echo "Directory ./$1 does not exist"
      exit 1
    fi
  else
    echo "Invalid argument $2"
    exit 1
  fi
fi

