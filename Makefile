 #
 # Project: Minesweper
 # Name: Makefile
 # Author: Tygan Chin
 # Summary: 
 #

############## Variables ###############

CXX = g++ # The compiler being used

# Updating include path to use Comp 40 .h files and CII interfaces
IFLAGS = -I/opt/homebrew/Cellar/sfml/2.6.1/include

# Compile flags
CXXFLAGS = -g3 -Wall -Wextra -Werror -Wfatal-errors -pedantic $(IFLAGS)

# Linking flags
LDFLAGS = -L/opt/homebrew/Cellar/sfml/2.6.1/lib

# Libraries needed for linking
LDLIBS = -g3 -lsfml-graphics -lsfml-window -lsfml-system

# Collect all .h files in your directory.
INCLUDES = $(shell echo *.h)

############### Rules ###############

## Compile step (.c files -> .o files)
%.o: %.cpp $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $< -o $@

## Linking step (.o -> executable program)
minesweeper: minesweeper.o board.o difficulty.o endingScreen.o
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

starting: testMain.o difficulty.o SFMLhelper.o
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

clean:
	rm -f minesweeper *.o