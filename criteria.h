#ifndef CRITERIA_H
#define CRITERIA_H

#include <vector>

#include "boardelement.h"
#include "subject.h"
#include "observer.h"
#include "state.h"

class Player;
class Goal;
class Visitor;

class Criteria : 
public BoardElement,
public Subject<ResourcesState>, 
public Subject<CompleteCriteriaState>, 
public Observer<ResourcesState>, 
public Observer<CompleteCriteriaState>, 
public Subject<DisplayState> {
	private:
		CompletionLevel completion;
		Player *owner;
		bool isAdjacent;
		std::vector<Goal*> goals;

		bool isConnected(Player*);

	public:
		Criteria(int location);

		void update(ResourcesState&);
		void update(CompleteCriteriaState&);

		void addGoal(Goal*);

		bool isOwnedBy(Player*);
		bool isComplete();

		int getValue();
		Player* getPlayer();
		Resources getCost();
		std::string getCompletion();

		void complete(Player*, bool force = false);
		void improve(Player*, bool force = false);

		std::string toStr();

		void accept(Visitor*);

};

#endif
