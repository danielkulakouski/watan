#ifndef TILE_H
#define TILE_H

#include <vector>
#include <string>

#include "boardelement.h"
#include "subject.h"
#include "observer.h"
#include "state.h"

class Criteria;
class Player;
class Visitor;

class Tile : 
public BoardElement,
public Subject<ResourcesState>, 
public Observer<RollState>, 
public Subject<DisplayState> {
	private:
		Resources resources;
		int value;
		bool hasGeese;
		std::vector<Criteria*> criteria;

	public:
		Tile(int location);

		Resources getResources();
		int getValue();

		void setResource(int value, int resourceId);
		void setResource(int value, std::string resource);

		void update(RollState&);

		void addCriteria(Criteria*);

		void addGeese();
		void removeGeese();

		std::vector<Player*> getPlayers();

		std::string valueStr();
		std::string resourceStr();

		void accept(Visitor*);

		std::string getResourcesType();

};

#endif
