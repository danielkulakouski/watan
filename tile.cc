#include <iostream>

#include "tile.h"
#include "criteria.h"
#include "visitor.h"

using namespace std;

Tile::Tile(int location): 
BoardElement{location},
resources{Resources{}},
value{0},
hasGeese{false},
criteria{vector<Criteria*>{}} {}

Resources Tile::getResources() {
	return resources;
}

int Tile::getValue() {
	return value;
}

void Tile::setResource(int value, int resourceId) {
	setResource(value, Resources::ResourceTypes[resourceId]);
}

void Tile::setResource(int value, string resource) {
	this->value = value;
	resources = Resources{resource};

	Subject<DisplayState>::notify(DisplayState{location, SubjectType::TILE, this});
}

void Tile::update(RollState &state) {
	if(state.roll == value) {
		if(hasGeese || value == 7) {
			return;
		}

		Subject<ResourcesState>::notify(ResourcesState{resources});
	}
}

void Tile::addCriteria(Criteria *criterion) {
	criteria.emplace_back(criterion);
}

void Tile::addGeese() {
	hasGeese = true;
}

void Tile::removeGeese() {
	hasGeese = false;
}

vector<Player*> Tile::getPlayers() {
	vector<Player*> result{};

	for(auto &crit: criteria) {
		if(crit->isComplete()) {
			result.emplace_back(crit->getPlayer());
		}
	}
	return result;
}

string Tile::valueStr() {
	if(value == 7) {
		return "";
	}
	return to_string(value);
}

void Tile::accept(Visitor *v) {
	v->visitTile(*this);
}

string Tile::getResourcesType() {
	return resources.getResourcesType();
}
