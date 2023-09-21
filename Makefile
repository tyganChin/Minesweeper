#########################################################
#                  Minesweeper Makefile                 #
#########################################################

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 

Minesweeper: 
	${CXX} ${LDFLAGS} -o MetroSim main.o MetroSim.o PassengerQueue.o Passenger.o

Minesweeper.o: minesweeper.cpp 
	$(CXX) $(CXXFLAGS) -c minesweeper.cpp

board.o: board.cpp board.h
	$(CXX) $(CXXFLAGS) -c Passenger.cpp

MetroSim.o: PassengerQueue.o MetroSim.h MetroSim.cpp 
	$(CXX) $(CXXFLAGS) -c MetroSim.cpp

main.o:	minesweeper.o mainMine.cpp
	$(CXX) $(CXXFLAGS) -c mainMine.cpp
					
clean: 
	rm *.o *~ a.out
