#ifndef DICE_H
#define DICE_H

#include "subject.h"
#include "state.h"

class Dice {
	public:
		virtual int roll() = 0;
};

#endif
