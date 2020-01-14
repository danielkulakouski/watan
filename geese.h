#ifndef GEESE_H
#define GEESE_H

#include "boardelement.h"
#include "subject.h"
#include "observer.h"
#include "state.h"

class Visitor;

class Geese : 
public BoardElement,
public Subject<GeeseState>, 
public Subject<DisplayState> {
	public:
		Geese();

		void discard();

		void move(int newLoc, bool force = false);

		void accept(Visitor*);

};

#endif
