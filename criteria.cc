#include "criteria.h"
#include "player.h"
#include "goal.h"
#include "exception.h"
#include "visitor.h"

using namespace std;

Criteria::Criteria(int location):
BoardElement{location},
completion{CompletionLevel::NOT},
owner{nullptr},
isAdjacent{false},
goals{vector<Goal*>{}} {}

int Criteria::getValue() {
	if(completion == CompletionLevel::ASSIGNMENT) {
		return 1;
	} else if(completion == CompletionLevel::MIDTERM) {
		return 2;
	} else if(completion == CompletionLevel::EXAM) {
		return 3;
	}  

	return 0;
}

void Criteria::update(ResourcesState &state) {
	Subject<ResourcesState>::notify(ResourcesState{state.resources * getValue()});
}

void Criteria::update(CompleteCriteriaState &state) {
	isAdjacent = true;
}

void Criteria::addGoal(Goal *goal) {
	goals.emplace_back(goal);
}

Resources Criteria::getCost() {
	return Resources{completion};
}

string Criteria::getCompletion() {
	if(completion == CompletionLevel::ASSIGNMENT) {
		return "Assignment";
	} else if(completion == CompletionLevel::MIDTERM) {
		return "Midterm";
	} else if(completion == CompletionLevel::EXAM) {
		return "Exam";
	}  
	return "Not";
}

bool Criteria::isConnected(Player* player) {
	for(auto &goal : goals) {
		if(goal->isOwnedBy(player)) {
			return true;
		}
	}
	return false;
}

bool Criteria::isOwnedBy(Player* player) {
	return owner == player;
}

bool Criteria::isComplete() {
	return completion != CompletionLevel::NOT;
}

Player *Criteria::getPlayer() {
	return owner;
}
		
void Criteria::complete(Player* player, bool force) {

	if(completion != CompletionLevel::NOT) {
		throw AlreadyCompletedException{"This criterion has already been completed"};
	}

	if(!isConnected(player) && !force) {
		throw NotConnectedException{"This criterion does not neighbour any of your goals!"};
	}

	if(isAdjacent && !force) {
		throw TooCloseException{"This criterion is too close to another criterion!"};
	}
	
	player->complete(this, force);

	completion = CompletionLevel::ASSIGNMENT;

	owner = player;

	Subject<CompleteCriteriaState>::notify(CompleteCriteriaState{});
	Subject<DisplayState>::notify(DisplayState{location, SubjectType::CRITERIA, this});
}
		
void Criteria::improve(Player* player, bool force) {

	if(completion == CompletionLevel::NOT) {
		throw WrongPlayerException{"This criterion has not been completed yet!"};
	} else if(completion == CompletionLevel::EXAM) {
		throw AlreadyMaxedException{"This criterion cannot be improved further!"};
	}

	if(owner != player) {
		throw WrongPlayerException{"This criterion is not yours!"};
	}

	player->improve(this, force);

	if(completion == CompletionLevel::ASSIGNMENT) {
		completion = CompletionLevel::MIDTERM;
	} else if(completion == CompletionLevel::MIDTERM) {
		completion = CompletionLevel::EXAM;
	}
	
	Subject<DisplayState>::notify(DisplayState{location, SubjectType::CRITERIA, this});
}

string Criteria::toStr() {
	if(completion == CompletionLevel::NOT) {
		return "XX";
	}

	string res = owner->toStr();

	if(completion == CompletionLevel::ASSIGNMENT) {
		res += "A";
	} else if(completion == CompletionLevel::MIDTERM) {
		res += "M";
	} else if(completion == CompletionLevel::EXAM) {
		res += "E";
	}

	return res;
}

void Criteria::accept(Visitor *v) {
	v->visitCriteria(*this);
}
