#ifndef LOADGAME_H
#define LOADGAME_H

#include "loadboard.h"

class Board;

class LoadGame : 
public LoadBoard {
	private:
		Board *board;

	public:
		LoadGame(std::string);

		void visitGame(Game&);
		void visitPlayer(Player&);
		void visitGeese(Geese&);
};

#endif
