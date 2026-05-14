# Makefile for IFAMDS Forest Management System
# Build both console and SFML versions

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
SFML_FLAGS = $(shell pkg-config --cflags --libs sfml-graphics sfml-window sfml-system)

# Source files
COMMON_SRCS = arraylayer.cpp linklist_layer.cpp monitoring_layer.cpp tree_layer.cpp \
              graph_layer.cpp queue_layer.cpp hash_layer.cpp stack_layer.cpp
SFML_SRCS = main.cpp
CONSOLE_SRCS = main_console.cpp

# Object files
COMMON_OBJS = $(COMMON_SRCS:.cpp=.o)
SFML_OBJS = $(SFML_SRCS:.cpp=.o)
CONSOLE_OBJS = $(CONSOLE_SRCS:.cpp=.o)

# Executables
SFML_EXE = sfml_menu
CONSOLE_EXE = ifamds_console

# Default target
all: $(SFML_EXE) $(CONSOLE_EXE)

# SFML executable
$(SFML_EXE): $(SFML_OBJS) $(COMMON_OBJS)
	$(CXX) -o $@ $^ $(SFML_FLAGS)

# Console executable
$(CONSOLE_EXE): $(CONSOLE_OBJS) $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Object files for SFML version
main.o: main.cpp
	$(CXX) $(CXXFLAGS) $(shell pkg-config --cflags sfml-graphics sfml-window sfml-system) -c $<

# Object files for console version
main_console.o: main_console.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Common object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

# Targets
.PHONY: all clean sfml console help

sfml: $(SFML_EXE)
	@echo "SFML version built: ./$(SFML_EXE)"

console: $(CONSOLE_EXE)
	@echo "Console version built: ./$(CONSOLE_EXE)"

run-sfml: $(SFML_EXE)
	./$(SFML_EXE)

run-console: $(CONSOLE_EXE)
	./$(CONSOLE_EXE)

clean:
	rm -f *.o $(SFML_EXE) $(CONSOLE_EXE)

help:
	@echo "Available targets:"
	@echo "  make              - Build both SFML and console versions"
	@echo "  make sfml         - Build only SFML version"
	@echo "  make console      - Build only console version"
	@echo "  make run-sfml     - Build and run SFML version"
	@echo "  make run-console  - Build and run console version"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make help         - Show this help message"
