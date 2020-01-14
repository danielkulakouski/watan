#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "element.h"
#include "state.h"
#include "tile.h"
#include "goal.h"
#include "criteria.h"
#include "observer.h"
#include "geese.h"

class Dice;
class Visitor;

class Board :
public Element {
	private:
		std::vector<Tile> tiles;
		std::vector<Goal> goals;
		std::vector<Criteria> criteria;

	public:
		Board(std::string mapFileName = "default.map");

		void generateResources();

		void attachDisplay(Observer<DisplayState>*);
		void attachDice(Subject<RollState>*);

		bool moveGeese(Geese &geese, int newLoc, bool force = false);

		std::vector<Tile> &getTiles();
		std::vector<Goal> &getGoals();
		std::vector<Criteria> &getCriteria();

		void accept(Visitor*);

};

#endif
