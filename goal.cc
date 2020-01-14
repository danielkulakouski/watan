#include "goal.h"
#include "exception.h"
#include "player.h"
#include "criteria.h"
#include "visitor.h"

using namespace std;

Goal::Goal(int location):
BoardElement{location},
owner{nullptr},
connectedGoals{vector<Goal*>{}},
connectedCriteria{vector<Criteria*>{}} {}

bool Goal::isOwnedBy(Player *player) {
	return owner == player;
}

int Goal::getColourId() {
	if(owner != nullptr) {
		return owner->getColourId();
	}
	return 37;
}

Resources Goal::getCost() {
	return Resources{*this};
}

bool Goal::isConnected(Player* player) {
	for(auto &goal : connectedGoals) {
		if(goal->isOwnedBy(player)) {
			return true;
		}
	}
	for(auto &criteria : connectedCriteria) {
		if(criteria->isOwnedBy(player)) {
			return true;
		}
	}
	return false;
}

void Goal::achieve(Player* player, bool force) {
	if(owner != nullptr) {
		throw AlreadyAchievedException{"This goal has already been achieved!"};
	}

	if(!isConnected(player) && !force) {
		throw NotConnectedException{"This goal does not neighbour any of your other goals or criteria!"};
	}

	player->achieve(this, force);

	owner = player;
	
	notify(DisplayState{location, SubjectType::GOAL, this});
}

void Goal::addCriteria(Criteria *criteria) {
	connectedCriteria.emplace_back(criteria);
}
		
void Goal::addGoal(Goal *goal) {
	connectedGoals.emplace_back(goal);
}

std::string Goal::toStr() {
	if(owner == nullptr) {
		return "XX";
	} else {
		return owner->toStr() + "A";
	}
}

void Goal::accept(Visitor *v) {
	v->visitGoal(*this);
}

