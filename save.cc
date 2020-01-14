
#include "save.h"
#include "game.h"
#include "board.h"
#include "tile.h"
#include "goal.h"
#include "criteria.h"
#include "player.h"
#include "geese.h"

using namespace std;

Save::Save(string fileName):
out{ofstream{fileName}} {}

void Save::visitGame(Game& game) {
	out << game.getCurTurn() << endl;
	for(auto &p : game.getPlayers()) {
		p.accept(this);
	}
	game.getBoard().accept(this);
	game.getGeese().accept(this);
}

void Save::visitBoard(Board& board) {
	bool first = true;
	for(auto &t : board.getTiles()) {
		if(!first) {
			out << " ";
		}
		first = false;
		t.accept(this);
	}
	out << endl;
}

void Save::visitTile(Tile& tile) {

	int value = tile.getValue();

	if(tile.getResources().getResourcesType() == "NETFLIX") {
		value = 7;
	}
	
	out << tile.getResources().getResourcesId() << " " << value;
}

void Save::visitGoal(Goal& goal) {
	out << " " << goal.getLocation();
}

void Save::visitCriteria(Criteria& criteria) {
	out << " " << criteria.getLocation() << " " << criteria.getValue();
}

void Save::visitPlayer(Player& player) {
	out << player.getResources().toStr(" ", true, false);
	out << " g";
	for(auto goal : player.getGoals()) {
		goal->accept(this);
	}
	out << " c";
	for(auto criterion : player.getCriteria()) {
		criterion->accept(this);
	}
	out << endl;
}

void Save::visitGeese(Geese& geese) {
	if(geese.getLocation() < 0) {
		out << endl;
	} else {
		out << geese.getLocation() << endl;
	}
}

void Save::saveToFile() {
	out.close();
}

