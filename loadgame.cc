#include <sstream>
#include <string>

#include "loadgame.h"
#include "game.h"
#include "board.h"
#include "tile.h"
#include "goal.h"
#include "criteria.h"
#include "player.h"
#include "geese.h"

using namespace std;

LoadGame::LoadGame(string fileName):
LoadBoard{fileName} {}

void LoadGame::visitGame(Game& game) {
	
	int curTurn;
	in >> curTurn;

	in.ignore(1);

	game.setCurTurn(curTurn);

	board = &game.getBoard();
	
	for(auto &p : game.getPlayers()) {
		p.accept(this);
	}

	game.getBoard().accept(this);

	game.getGeese().accept(this);

	in.close();
}

void LoadGame::visitPlayer(Player& player) {
	string line;

	getline(in, line);
	
	vector<int> res{};

	istringstream sin{line};

	int loc, value;
	string s;

	while((sin >> s) && s != "g") {
		
		res.emplace_back(stoi(s));
	}

	player.setResources(Resources{res});

	while((sin >> s) && s != "c") {
		loc = stoi(s);

		Goal &goal = board->getGoals()[loc];

		goal.achieve(&player, true);	
	}

	while(sin >> loc >> value) {
		Criteria &criterion = board->getCriteria()[loc];

		criterion.complete(&player, true);

		for(int i=1;i<value;i++) {
			criterion.improve(&player, true);
		}
	}
}

void LoadGame::visitGeese(Geese& geese) {
	int loc;
	if(in >> loc) {
		board->moveGeese(geese, loc);
	}
}
