#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <memory>

#include "element.h"
#include "subject.h"
#include "resources.h"
#include "observer.h"
#include "state.h"
#include "dice.h"
#include "geese.h"

class Goal;
class Criteria;
class Visitor;

class Player : 
public Element,
public Observer<ResourcesState>, 
public Observer<GeeseState>, 
public Subject<RollState> {
	private:
	
		static const std::vector<std::string> PlayerColours;
		//const std::vector<int>PlayerColoursIds{34, 31, 91, 93, 96, 95, 92, 97};
		static const std::vector<int> PlayerColoursIds;

		int id;

		std::string colour;
		Resources resources;
		std::shared_ptr<Dice> dice;
		std::vector<Goal*> goals;
		std::vector<Criteria*> criteria;

		Resources gainedThisTurn;

	public:
		Player(int id);

		void update(ResourcesState&) override;
		void update(GeeseState&) override;

		void setResources(Resources);

		void chooseFairDice();
		void chooseLoadedDice(int value);
		int roll();

		std::string steal(Player*);
		void trade(Player*, std::string, std::string);

		Resources getResources();
		std::vector<Goal*> &getGoals();
		std::vector<Criteria*> &getCriteria();

		int getCompletedCriteria();
		std::string getStatus();
		Dice* getDice();
		std::string toStr();
		std::string getColour();
		std::string getCriteriaList(std::string sep = " ");
		int getColourId();

		bool gotThisTurn();

		void achieve(Goal*, bool force = false);
		void complete(Criteria*, bool force = false);
		void improve(Criteria*, bool force = false);

		void attachToDice(Observer<RollState>*);

		void accept(Visitor*);

};

#endif
