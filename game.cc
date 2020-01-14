#include <iostream>
#include <algorithm>
#include <set>

#include "game.h"
#include "save.h"
#include "exception.h"

using namespace std;

Game::Game(int playerCnt, string mapFile, int winLimit, bool colour): 
board{Board{mapFile}}, 
players{vector<Player>{}}, 
geese{Geese{}}, 
textDisplay{TextDisplay{mapFile, colour}}, 
playerCnt{playerCnt},
curTurn{0},
winLimit{winLimit} {
	
	players.reserve(playerCnt);

	for(int i=0;i<playerCnt;i++) {
		players.emplace_back(Player{i});
		//players[i].attachToDice(&geese);
		geese.Subject<GeeseState>::attach(&players[i]);
		board.attachDice(&players[i]);
	}

	board.attachDisplay(&textDisplay);
	geese.Subject<DisplayState>::attach(&textDisplay);

	board.generateResources();
}

void Game::printStatus() {
	for(int i=0;i<playerCnt;i++) {
		cout << players[i].getStatus() << endl;
	}
}

vector<string> Game::START_OF_TURN_COMMANDS{"load", "fair", "roll"};
vector<string> Game::END_OF_TURN_COMMANDS{"board", "status", "criteria", "achieve", "complete", "improve", "trade", "next", "save", "help"};

int readInt() {
	int n;
	while(!(cin >> n) || n < 0){
		if(cin.eof()) {
			return -1;
		}
		cin.clear();
		cin.ignore(1000, '\n');
		cout << "Please input a non negative integer." << endl << ">";
	}
	return n;
}

string toLower(string s) {
	for(auto &c : s) {
		if('A' <= c && c <= 'Z') {
			c += 'a' - 'A';
		}
	}
	return s;
}

string toUpper(string s) {
	for(auto &c : s) {
		if('a' <= c && c <= 'z') {
			c += 'A' - 'a';
		}
	}
	return s;
}

string matchCommand(string s, vector<string> &commands) {

	string res = "invalid";

	for(auto com : commands) {
		if(s.length() <= com.length()) {
			if (mismatch(s.begin(), s.end(), com.begin()).first == s.end()) {
				if(res == "invalid") {
					res = com;
				} else {
					return "invalid";
				}
			}
		}
	}

	return res;
}

void Game::firstTurn() {
	for(int i=0;i<playerCnt*2;i++) {
		curTurn = i >= playerCnt ? 2 * playerCnt - i - 1 : i;

		Player &curPlayer = players[curTurn];

		cout << textDisplay << endl;
		cout << "Student " << curPlayer.getColour() << ", where do you want to complete an assignment?"; 

		bool completed = false;

		while(!completed) {

			cout << endl << ">";

			int n = readInt();

			if(n == -1) {
				return;
			}
			
			vector<Criteria> &criteria = board.getCriteria();

			if(n < 0 || n >= criteria.size()) {
				cout << "This criterion does not exist!";
				continue;
			}

			try {
				criteria[n].complete(&curPlayer, true);
				completed = true;
			} catch(const NotEnoughResourcesException &e) {
				cout << e.what() << endl;
			} catch(const AlreadyMaxedException &e) {
				cout << e.what() << endl;
			} catch(const NotConnectedException &e) {
				cout << e.what() << endl;
			} catch(const TooCloseException &e) {
				cout << e.what() << endl;
			} catch(const AlreadyCompletedException &e) {
				cout << e.what() << endl;
			} catch(const WrongPlayerException &e) {
				cout << e.what() << endl;
			}
		}
	}
}

bool Game::turn() {

	Player &curPlayer = players[curTurn];

	cout << textDisplay << endl;
	cout << "Student " << curPlayer.getColour() << "'s turn:" << endl;
	cout << curPlayer.getStatus() << endl;

	string s;
	int n, roll = -1;

	while(roll == -1) {
		cout << ">";
		if(!(cin >> s)) {
			return true;
		}
		s = matchCommand(toLower(s), Game::START_OF_TURN_COMMANDS);

		if(s == "load") {
			cout << "Input a roll between 2 and 12:" << endl << ">";
			int value = readInt();
			while(value < 2 || value > 12) {

				if(value == -1) {
					return true;
				}

				cout << "Invalid roll!" << endl << ">";
				value = readInt();
			}
			curPlayer.chooseLoadedDice(value);
		} else if(s == "fair") {
			curPlayer.chooseFairDice();
		} else if(s == "roll") {
			roll = curPlayer.roll();
		} else {
			cout << "You can only do roll/fair/load!" << endl;
		}
	}

	//int roll = curPlayer.roll();

	if(roll == 7) {
		geese.discard();
		cout << "Choose where to place the GEESE." << endl << ">";
		int newLoc = readInt();
		while(!board.moveGeese(geese, newLoc)) {
			if(newLoc == -1) {
				return true;
			}
			cout << ">";
			newLoc = readInt();
		}

		vector<Tile> &tiles = board.getTiles();

		vector<Player*> victims = tiles[geese.getLocation()].getPlayers();

		victims.erase(remove(victims.begin(), victims.end(), &curPlayer), victims.end());

		set<Player*> s(victims.begin(), victims.end());
		victims.assign(s.begin(), s.end());

		for(int i=victims.size()-1;i>=0;i--) {
			if(victims[i]->getResources().total() == 0) {
				victims.erase(victims.begin() + i);
			}
		}

		if(victims.size() == 0) {
			cout << "There is no one to steal from." << endl;
		} else {

			Player *chosenPlayer = nullptr;

			if(victims.size() > 1) {

				cout << "Student " << curPlayer.getColour() << " can choose to steal from ";

				bool first = true;

				for(auto vic : victims) {
					if(!first) {
						cout << ", ";
					}
					first = false;
					cout << vic->getColour();
				}

				cout << "." << endl;

				string chosen;
				bool isPlayer = false;

				while(!isPlayer) {
					cout << ">";
					if(!(cin >> chosen)) {
						return true;
					}
					for(auto vic : victims) {
						if(chosen == vic->getColour()) {
							isPlayer = true;
							chosenPlayer = vic;
						}
					}
				}
			} else {
				chosenPlayer = victims[0];
				cout << "Student " << curPlayer.getColour() << " can only steal from student " << chosenPlayer->getColour() << "." << endl;
			}

			string stolen = curPlayer.steal(chosenPlayer);

			cout << "Student " << curPlayer.getColour() << " steals " << stolen << " from student " << chosenPlayer->getColour() << "." << endl;
		}
	} else {
		bool noOneGotAnything = true;
		for(auto &player : players) {
			if(player.gotThisTurn()) {
				noOneGotAnything = false;
			}
		}
		if(noOneGotAnything) {
			cout << "No students gained resources." << endl;
		}
	}

	while(!cin.eof()) {

		if(curPlayer.getCompletedCriteria() >= winLimit) {

			cout << curPlayer.getColour() << " won!!!" << endl;

			return true;
		}

		cout << ">";

		if(!(cin >> s)) {
			return true;
		}

		s = matchCommand(toLower(s), Game::END_OF_TURN_COMMANDS);
		
		if(s == "board") {
			cout << textDisplay << endl;
		} else if(s == "status") {
			printStatus();
		} else if(s == "criteria") {
			cout << curPlayer.getCriteriaList("\n") << endl;
		} else if(s == "achieve") {
			n = readInt();

			vector<Goal> &goals = board.getGoals();

			if(n < 0 || n >= goals.size()) {
				cout << "This goal does not exist!";
				continue;
			}

			try {
				goals[n].achieve(&curPlayer);
			} catch(const NotEnoughResourcesException &e) {
				cout << e.what() << endl;
			} catch(const AlreadyAchievedException &e) {
				cout << e.what() << endl;
			} catch(const NotConnectedException &e) {
				cout << e.what() << endl; 
			}
		} else if(s == "complete" || s == "improve") {
			n = readInt();

			vector<Criteria> &criteria = board.getCriteria();

			if(n < 0 || n >= criteria.size()) {
				cout << "This criterion does not exist!";
				continue;
			}

			try {
				if(s == "complete") {
					criteria[n].complete(&curPlayer);
				} else {
					criteria[n].improve(&curPlayer);
				}
			} catch(const NotEnoughResourcesException &e) {
				cout << e.what() << endl;
			} catch(const AlreadyMaxedException &e) {
				cout << e.what() << endl;
			} catch(const NotConnectedException &e) {
				cout << e.what() << endl;
			} catch(const TooCloseException &e) {
				cout << e.what() << endl;
			} catch(const AlreadyCompletedException &e) {
				cout << e.what() << endl;
			} catch(const WrongPlayerException &e) {
				cout << e.what() << endl;
			}
		} else if(s == "trade") {
			// complicated code goes here:
			// ... left as excerise to the marker

			Player* otherPlayer = nullptr;

			string chosen, give, take;

			if(!(cin >> chosen >> give >> take)) {
				return true;
			}
			for(auto &p : players) {
				if(chosen == p.getColour()) {
					otherPlayer = &p;
				}
			}
			if(otherPlayer == nullptr) {
				cout << "Player does not exist!" << endl;
			} else if(otherPlayer == &curPlayer) {
				cout << "You cannot trade with yourself!" << endl;
			} else if(!Resources::isType(give) || !Resources::isType(take)) {
				cout << "That is not a valid resource!" << endl;
			} else {
				curPlayer.trade(otherPlayer, give, take);
			}

		} else if(s == "next") {
			break;
		} else if(s == "save") {

			string fname;
			if(!(cin >> fname)) {
				return true;
			}

			Save save{fname};

			accept(&save);

			save.saveToFile();

		} else if(s == "help") {
			cout << "Valid Commands:" << endl;
			cout << "board" << endl;
			cout << "status" << endl;
			cout << "criteria" << endl;
			cout << "achieve <goal>" << endl;
			cout << "complete <criterion>" << endl;
			cout << "improve <criterion>" << endl;
			cout << "trade <colour> <give> <take>" << endl;
			cout << "next" << endl;
			cout << "save <file>" << endl;
			cout << "help" << endl;
		 } else {
			cout << "Invalid command!" << endl;
		 }

		s = "";
	}

	curTurn = (curTurn + 1) % playerCnt;

	if(cin.eof()) {
		return true;
	}
	return false;
}

int Game::getCurTurn() {
	return curTurn;
}

void Game::setCurTurn(int ct) {
	curTurn = ct;
}

Board &Game::getBoard() {
	return board;
}

vector<Player> &Game::getPlayers() {
	return players;
}

Geese &Game::getGeese() {
	return geese;
}

void Game::accept(Visitor *v) {
	v->visitGame(*this);
}
