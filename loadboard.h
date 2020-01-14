#ifndef LOADBOARD_H
#define LOADBOARD_H

#include <string>
#include <fstream>

#include "visitor.h"

class LoadBoard : public Visitor {
	protected:
		std::ifstream in;

	public:
		LoadBoard(std::string);

		virtual void visitGame(Game&);
		virtual void visitBoard(Board&);
		virtual void visitTile(Tile&);
		virtual void visitGoal(Goal&);
		virtual void visitCriteria(Criteria&);
		virtual void visitPlayer(Player&);
		virtual void visitGeese(Geese&);
};

#endif
