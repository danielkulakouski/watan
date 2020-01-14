#ifndef GAME_H
#define GAME_H

#include <vector>
#include <memory>

#include "element.h"
#include "board.h"
#include "player.h"
#include "geese.h"
#include "textdisplay.h"

class Visitor;

class Game :
public Element {
	private:
		Board board;
		std::vector<Player> players;
		Geese geese;

		TextDisplay textDisplay;

		int playerCnt;

		int curTurn;

		int winLimit;

		void printStatus();

		static std::vector<std::string> START_OF_TURN_COMMANDS;
		static std::vector<std::string> END_OF_TURN_COMMANDS;

	public:
		Game(int playerCnt = 4, std::string mapFile = "default.map", int winLimit = 10, bool colour = false);

		void firstTurn();
		bool turn();

		int getCurTurn();
		void setCurTurn(int);
		Board &getBoard();
		std::vector<Player> &getPlayers();
		Geese &getGeese();

		void accept(Visitor*);

};

#endif
