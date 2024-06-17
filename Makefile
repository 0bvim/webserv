# Compiler
CXX := c++

# Directories
SRCDIR := src
INCDIR := include
BUILDDIR := build
TARGETDIR := bin

# Target executable name
TARGET := webserv

# Source files
SRCEXT := cpp
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))

# Object files
OBJECTS := $(patsubst $(SRCDIR)/%,$(BUILDDIR)/%,$(SOURCES:.$(SRCEXT)=.o))

# Includes
INC := -I$(INCDIR)

# Flags
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror

# Final executable
EXECUTABLE := $(TARGETDIR)/$(TARGET)

# Default make target
all: directories $(EXECUTABLE)

# Ensure build directories exist
directories:
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(TARGETDIR)

# Rule for linking object files into executable
$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $^ -o $(EXECUTABLE)

# Rule for compiling source files into object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	$(CXX) $(CXXFLAGS) $(INC) -c -o $@ $<

# Clean build artifacts
clean:
	@rm -rf $(BUILDDIR)
	@rm -rf $(TARGETDIR)

# Run the executable
run: clean all
	@./$(EXECUTABLE) $(ARGS)

fclean: clean
	@rm -rf $(TARGETDIR)/$(TARGET)

re: fclean all

test:
	@ echo "\nRunning test with no argument"
	@./$(EXECUTABLE)
	@ echo "\nRunning test with no file argument"
	@./$(EXECUTABLE) "nan"
	@ echo "\nRunning test with a valid file argument"
	@./$(EXECUTABLE) "data.csv"

val: clean all
	@valgrind ./$(EXECUTABLE) $(ARGS)

# Phony targets
.PHONY: all clean run
