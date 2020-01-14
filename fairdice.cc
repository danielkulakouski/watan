#include "fairdice.h"
#include <cstdlib>

FairDice::FairDice() {}

int FairDice::roll() {
	int res = (rand() % 6) + (rand() % 6) + 2;

	return res;
}
