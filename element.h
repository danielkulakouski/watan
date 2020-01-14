#ifndef ELEMENT_H
#define ELEMENT_H

class Visitor;
class Save;

class Element {
	public:
		virtual void accept(Visitor*) = 0;

	friend Visitor;
	friend Save;
};

#endif
