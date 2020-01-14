CXX = g++ # which C++ compiler to use
CXXFLAGS = -std=c++14 -Wall -MMD  # which C++ compiler flags to use
OBJECTS = main.o board.o criteria.o fairdice.o game.o geese.o goal.o loadeddice.o player.o resources.o textdisplay.o tile.o save.o loadboard.o loadgame.o # list all of the object files
DEPENDS = ${OBJECTS:.o=.d} # for each foo.o, there's a dep file foo.d
EXEC = watan # name of the final executable you picked

${EXEC}: ${OBJECTS} # if you just call "make", this will be the target
	${CXX} ${CXXFLAGS} ${OBJECTS} -o ${EXEC}

.PHONY: clean # "clean" is a task, but not a file name

clean:
	rm ${OBJECTS} ${EXEC} ${DEPENDS}

-include ${DEPENDS} # sucks in all of the generated .d files;
