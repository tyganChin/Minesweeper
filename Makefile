 #
 # Project: minesweper
 # Name: Makefile
 # Author: Tygan Chin
 # Purpose: Builds the minesweeper game using the necessary files and paths. 
 # 		    Type "make" to build the program executable 
 # Note: The include and linking paths must be updated to point to the SFML
 #  	 library on local computer.

############## Variables ###############

# The compiler being used
CXX = clang++

# Include flags
IFLAGS = -I/PATH_T0_SFML/inlclude

# Compile flags
CXXFLAGS = -g3 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
LDFLAGS = -L/PATH_TO_SFML/lib

# Libraries needed for linking
LDLIBS = -g3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# Define directory for .o, .cpp, and .h files
OBJ_DIR = Files_o
SRC_DIR = Files_cpp
HD_DIR  = Files_h

# Collect all of the .cpp files in cpp directory
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

# Collect all .h files in the header directory.
INCLUDES = $(wildcard $(HD_DIR)/*.h)

# Object files derived from source files
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))


############### Rules ###############

# Compile the .cpp files to .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCLUDES) | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Linking step (.o -> executable program)
minesweeper: $(OBJS)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f minesweeper $(OBJS)
	