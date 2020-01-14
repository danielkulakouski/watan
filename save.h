#ifndef SAVE_H
#define SAVE_H

#include <iostream>
#include <fstream>
#include <string>

#include "visitor.h"


class Save : public Visitor {
	private:
		std::ofstream out;
		
	public:
		Save(std::string);

		void visitGame(Game&);
		void visitBoard(Board&);
		void visitTile(Tile&);
		void visitGoal(Goal&);
		void visitCriteria(Criteria&);
		void visitPlayer(Player&);
		void visitGeese(Geese&);

		void saveToFile();
};

#endif
