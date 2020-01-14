#ifndef GOAL_H
#define GOAL_H

#include <vector>

#include "boardelement.h"
#include "subject.h"
#include "observer.h"
#include "state.h"
#include "state.h"

class Player;
class Criteria;
class Visitor;

class Goal :
public BoardElement,
public Subject<DisplayState> {
	private:
		Player *owner;
		std::vector<Goal*> connectedGoals;
		std::vector<Criteria*> connectedCriteria;

		bool isConnected(Player*);

	public:
		Goal(int location);

		bool isOwnedBy(Player*);

		int getColourId();

		Resources getCost();
		void achieve(Player*, bool force = false);

		void addCriteria(Criteria*);
		void addGoal(Goal*);

		std::string toStr();

		void accept(Visitor*);

};

#endif
