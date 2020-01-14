#include <iostream>

#include "geese.h"
#include "exception.h"
#include "visitor.h"

using namespace std;

Geese::Geese():
BoardElement{-1} {}

void Geese::discard() {
	Subject<GeeseState>::notify(GeeseState{}); 
}

void Geese::move(int newLoc, bool force) {
	location = newLoc;
	Subject<DisplayState>::notify(DisplayState{location, SubjectType::GEESE, this});
}

void Geese::accept(Visitor *v) {
	v->visitGeese(*this);
}
