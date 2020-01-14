#include <iostream>

#include "loadboard.h"
#include "game.h"
#include "board.h"
#include "tile.h"
#include "goal.h"
#include "criteria.h"
#include "player.h"
#include "geese.h"

using namespace std;

LoadBoard::LoadBoard(string fileName):
in{ifstream{fileName}} {}

void LoadBoard::visitGame(Game& game) {
	
	game.getBoard().accept(this);

	in.close();
}

void LoadBoard::visitBoard(Board& board) {
	for(auto &t : board.getTiles()) {
		t.accept(this);
	}
}

void LoadBoard::visitTile(Tile& tile) {
	int resId, value;
	in >> resId >> value;

	tile.setResource(value, resId);
}

void LoadBoard::visitGoal(Goal& goal) {

}

void LoadBoard::visitCriteria(Criteria& criteria) {

}

void LoadBoard::visitPlayer(Player& player) {

}

void LoadBoard::visitGeese(Geese& geese) {

}

