#ifndef FAIR_H
#define FAIR_H

#include "dice.h"

class FairDice : public Dice {
	public:
		FairDice();
		int roll();
};

#endif
