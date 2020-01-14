#ifndef LOADEDDICE_H
#define LOADEDDICE_H

#include "dice.h"

class LoadedDice : public Dice {
	private:
		int value;
	public:
		LoadedDice(int value);
		int roll();
};

#endif
