#include <iostream>
#include <sstream>

#include "player.h"
#include "goal.h"
#include "criteria.h"
#include "exception.h"
#include "fairdice.h"
#include "loadeddice.h"
#include "visitor.h"

using namespace std;

const vector<string> Player::PlayerColours{"Blue", "Red", "Orange", "Yellow", "Cyan", "Magenta", "Green", "White"};
const vector<int> Player::PlayerColoursIds{27, 196, 208, 220, 51, 200, 46, 255};

Player::Player(int id):
id{id},
colour{PlayerColours[id]},
resources{Resources{vector<int>{0, 0, 0, 0, 0, 0}}},
dice{make_shared<FairDice>()},
goals{vector<Goal*>{}},
criteria{vector<Criteria*>{}},
gainedThisTurn{Resources{}} {}

void Player::update(ResourcesState &state) {

	gainedThisTurn = gainedThisTurn + state.resources;

	resources = resources + state.resources;
}

void Player::update(GeeseState &state) {

	int total = resources.total();

	if(total >= 10) {
		cout << "Student " << colour << " loses " << (total/2);
		cout << " resources to the geese. They lose:" << endl;
		cout << resources.discard(total / 2).toStr("\n", false) << endl;
	}
}

void Player::setResources(Resources res) {
	resources = res;
}

void Player::chooseFairDice() {
	dice = make_shared<FairDice>();
}

void Player::chooseLoadedDice(int value) {
	dice = make_shared<LoadedDice>(value);
}

int Player::roll() {

	int res = dice->roll();

	cout <<	colour << " rolled " << res << endl;

	notify(RollState{res});

	return res;
}

string Player::steal(Player *other) {
	if(other->resources.total()==0) {
		return "";
	}

	Resources stolen = other->resources.discard(1);

	resources = resources + stolen;

	return stolen.getResourcesType();
}

void Player::trade(Player *other, string give, string take) {
	Resources giveRes{give};
	Resources takeRes{take};

	if(!(giveRes <= resources)) {
		cout << "You don't have 1 " << give << "!" << endl;
		return;
	} else if(!(takeRes <= other->resources)) {
		cout << other->getColour() << " doesn't have 1 " << take << "!" << endl;
		return;
	} else {
		cout << colour << " offers ";
		cout << other->getColour() << " one ";
		cout << give << " for one ";
		cout << take << "." << endl << "Does ";
		cout << other->getColour() << " accept this offer? (yes/no)" << endl << ">";

		string s;
		cin >> s;
		while(s != "yes" && s != "no" && s != "y" && s != "n") {
			cout << ">";
			cin >> s;
		}
		
		if(s == "yes" || s == "y") {
			other->resources = other->resources - takeRes + giveRes;
			resources = resources + takeRes - giveRes;
			cout << "Trade completed!" << endl;
			return;
		} else if(s == "no" || s == "n") {
			cout << other->getColour() << " declined!" << endl;
			return;
		}
	}
}

Resources Player::getResources() {
	return resources;
}

std::vector<Goal*> &Player::getGoals() {
	return goals;
}

std::vector<Criteria*> &Player::getCriteria() {
	return criteria;
}

int Player::getCompletedCriteria() {
	int res = 0;
	for(auto criterion : criteria) {
		res += criterion->getValue();
	}
	return res;
}

std::string Player::getStatus() {
	ostringstream out;

	out << colour << " has ";
	out << getCompletedCriteria() << " course criteria completions, ";
	out << resources.toStr(", ") << ".";

	return out.str();
}

Dice* Player::getDice() {
	return dice.get();
}

string Player::toStr() {
	return colour.substr(0, 1);
}

string Player::getColour() {
	return colour;
}

string Player::getCriteriaList(string sep) {
	ostringstream out;

	for(auto &criterion : criteria) {
		out << criterion->getLocation() << " " << criterion->getValue() << sep;
	}

	string res = out.str();

	if(res.length() > 0) {
		return res.substr(0, res.length() - sep.length());
	}

	return res;
}

int Player::getColourId() {
	return PlayerColoursIds[id];
}

bool Player::gotThisTurn() {
	if(gainedThisTurn.total() > 0) {

		cout << "Student " << colour << " gained:" << endl;
		cout << gainedThisTurn.toStr("\n", false) << endl;

		gainedThisTurn = Resources{};

		return true;
	}
	return false;
}

void Player::achieve(Goal* goal, bool force) {

	if(!force) {
		Resources cost = goal->getCost();

			if(!(resources >= cost)) {
			throw NotEnoughResourcesException{"Not enough resources!"};
		}

		resources = resources - cost;
	}

	goals.emplace_back(goal);
}

void Player::complete(Criteria* criteria, bool force) {
	
	if(!force) {

		Resources cost = criteria->getCost();

		if(!(resources >= cost)) {
			throw NotEnoughResourcesException{"Not enough resources!"};
		}

		resources = resources - cost;
	}

	criteria->Subject<ResourcesState>::attach(this);	
	this->criteria.emplace_back(criteria);
}

void Player::improve(Criteria* criteria, bool force) {
	
	if(!force) {
		Resources cost = criteria->getCost();

		if(!(resources >= cost)) {
			throw NotEnoughResourcesException{"Not enough resources!"};
		}

		resources = resources - cost;
	}
}

void Player::attachToDice(Observer<RollState>* observer) {
	attach(observer);
}

void Player::accept(Visitor *v) {
	v->visitPlayer(*this);
}
