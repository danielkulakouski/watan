#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>

#include "board.h"
#include "dice.h"
#include "subject.h"
#include "visitor.h"

using namespace std;

Board::Board(string mapFileName):
tiles{vector<Tile>{}},
goals{vector<Goal>{}},
criteria{vector<Criteria>{}} {
	ifstream in{mapFileName};

	// load number of tiles, goals, and criteria
	int tCnt,gCnt,cCnt;
	in >> tCnt >> gCnt >> cCnt;

	tiles.reserve(tCnt);
	goals.reserve(gCnt);
	criteria.reserve(cCnt);

	for(int i=0;i<tCnt;i++) {
		tiles.emplace_back(Tile{i});
	}

	for(int i=0;i<gCnt;i++) {
		goals.emplace_back(Goal{i});
	}

	for(int i=0;i<cCnt;i++) {
		criteria.emplace_back(Criteria{i});
	}

	// load criteria connections
	for(int ci=0;ci<cCnt;ci++) {
		int conCnt, i;

		in >> i;

		//cout << ci << " " << i << endl;

		// to tile
		in >> conCnt;

		for(int j=0;j<conCnt;j++) {
			int con;
			in >> con;
			Criteria *crit = &criteria[i];
			tiles[con].Subject<ResourcesState>::attach(crit);
			tiles[con].addCriteria(crit);
		}

		vector<int> connectedGoals{};

		// to goal
		in >> conCnt;

		for(int j=0;j<conCnt;j++) {
			int con;
			in >> con;
			criteria[i].addGoal(&goals[con]);
			goals[con].addCriteria(&criteria[i]);
			connectedGoals.emplace_back(con);
		}

		for(int j=0;j<conCnt;j++) {
			for(int k=0;k<conCnt;k++) {
				if(connectedGoals[j] != connectedGoals[k]) {
					goals[connectedGoals[j]].addGoal(&goals[connectedGoals[k]]);
				}
			}
		}
		
		// to neighbour criteria
		in >> conCnt;

		for(int j=0;j<conCnt;j++) {
			int con;
			in >> con;
			criteria[i].Subject<CompleteCriteriaState>::attach(&criteria[con]);
		}

	}

	in.close();
}

void Board::generateResources() {
	
	vector<int> valueCnt{2, 3, 3, 4, 4, 5, 5, 6, 6, 8, 8, 9, 9, 10, 10, 11, 11, 12};
	vector<string> resourceCnt{"TUTORIAL", "TUTORIAL", "TUTORIAL", "STUDY", "STUDY", "STUDY", "CAFFEINE", "CAFFEINE", "CAFFEINE", "CAFFEINE", "LAB", "LAB", "LAB", "LAB", "LECTURE", "LECTURE", "LECTURE", "LECTURE", "NETFLIX"};
	
	for(auto &tile : tiles) {
		int r2 = rand() % resourceCnt.size();
		if(resourceCnt[r2] == "NETFLIX") {
			tile.setResource(7, resourceCnt[r2]);
		} else {
			int r1 = rand() % valueCnt.size();
			tile.setResource(valueCnt[r1], resourceCnt[r2]);
			valueCnt.erase(valueCnt.begin() + r1);
		}
		resourceCnt.erase(resourceCnt.begin() + r2);
	}
}

void Board::attachDisplay(Observer<DisplayState> *display) {
	for(auto &tile : tiles) {
		tile.Subject<DisplayState>::attach(display);
	}

	for(auto &goal : goals) {
		goal.Subject<DisplayState>::attach(display);
	}

	for(auto &criterion : criteria) {
		criterion.Subject<DisplayState>::attach(display);
	}
}

void Board::attachDice(Subject<RollState> *player) {
	for(auto &tile : tiles) {
		player->Subject<RollState>::attach(&tile);
	}
}

bool Board::moveGeese(Geese &geese, int newLoc, bool force) {

	if(newLoc < 0 || newLoc >= tiles.size()) {
		cout << "Tile does not exist!" << endl;
		return false;
	}

	int oldLoc = geese.getLocation();

	if(oldLoc == newLoc) {
		cout << "The GEESE is already on that tile!" << endl;
		return false;
	}

	if(oldLoc >= 0) {
		tiles[oldLoc].removeGeese();
	}
	geese.move(newLoc, force);
	tiles[newLoc].addGeese();

	return true;	
}

vector<Tile> &Board::getTiles() {
	return tiles;
}
		
vector<Goal> &Board::getGoals() {
	return goals;
}
		
vector<Criteria> &Board::getCriteria() {
	return criteria;
}

void Board::accept(Visitor *v) {
	v->visitBoard(*this);
}
