#---------------------------------------------------------------------------
# File       : Makefile
# Content    : Compiles the program
# Author     : Wesley Marques (wesleymarques@dcc.ufmg.br)
#---------------------------------------------------------------------------
# Options    : make all      - compiles the program
#            : make run      - runs the program
#            : make clean    - removes object files and executable
#            : make valgrind - runs valgrind
#---------------------------------------------------------------------------

CC = g++
LIBS = -lm
SRC = src
OBJ = obj
INC = include
BIN = bin
CFLAGS = -I$(INC) -Wall -Wextra -Wpedantic -Wformat-security -Wconversion -Werror -std=c++17

# program name (executable)
NAME = main

# source file extension
END = cpp

# list all source files (all .cpp files inside src)
SRCS = $(wildcard $(SRC)/*.$(END))

# list of object files corresponding to source files in obj/
OBJS = $(patsubst $(SRC)/%.$(END),$(OBJ)/%.o,$(SRCS))

# full path for executable
EXE = $(BIN)/$(NAME)

# default rule: compile everything and create executable
all: $(EXE)

# link object files to create executable
$(EXE): $(OBJS) | $(BIN)
	@$(CC) $(OBJS) -o $(EXE) $(LIBS)

# compile each .cpp file into .o, including headers from include/
$(OBJ)/%.o: $(SRC)/%.$(END) | $(OBJ)
	@$(CC) $(CFLAGS) -c $< -o $@

# create obj directory if it doesn't exist
$(OBJ):
	@mkdir -p $(OBJ)

# create bin directory if it doesn't exist
$(BIN):
	@mkdir -p $(BIN)

# run the program (accepts command line args)
run: $(EXE)
	@$(EXE) $(ARGS)

# run valgrind to detect leaks (accepts command line args)
valgrind: $(EXE)
	@valgrind --leak-check=full --show-leak-kinds=all $(EXE) $(ARGS)

# remove object files and executable
clean:
	@rm -rf $(OBJ)/*.o $(EXE) gmon.out
	@rm -rf $(OBJ)/ $(BIN)/
