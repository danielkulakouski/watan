#ifndef BOARDELEMENT_H
#define BOARDELEMENT_H

#include "element.h"

class BoardElement :
public Element {
	protected:
		int location;
	public:
		BoardElement(int location): location{location} {}

		virtual int getLocation() {
			return location;
		}
};

#endif
