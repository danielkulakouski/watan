#ifndef VISITOR_H
#define VISITOR_H

class Game;
class Board;
class Tile;
class Goal;
class Criteria;
class Player;
class Geese;

class Visitor {
	public:
		virtual void visitGame(Game&) = 0;
		virtual void visitBoard(Board&) = 0;
		virtual void visitTile(Tile&) = 0;
		virtual void visitGoal(Goal&) = 0;
		virtual void visitCriteria(Criteria&) = 0;
		virtual void visitPlayer(Player&) = 0;
		virtual void visitGeese(Geese&) = 0;
};

#endif
