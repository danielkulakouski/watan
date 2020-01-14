#ifndef STATE_H
#define STATE_H

#include <string>

#include "resources.h"

enum SubjectType{CRITERIA, GOAL, TILE, GEESE};

struct RollState {
	int roll;
	RollState(int roll): roll{roll} {}
};

struct ResourcesState {
	Resources resources;
	ResourcesState(Resources resources): resources{resources} {}
};

struct CompleteCriteriaState {};

struct GeeseState {};

template <typename StateType> class Subject;

struct DisplayState {

	int location;
	SubjectType sType;
	Subject<DisplayState> *subject;

	DisplayState(int location, SubjectType sType, Subject<DisplayState> *subject):
	location{location}, sType{sType}, subject{subject} {}
};

#endif
