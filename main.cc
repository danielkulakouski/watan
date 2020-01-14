#include <iostream>
#include <string>
#include <cstdlib>

#include "game.h"
#include "loadgame.h"

using namespace std;

int main(int argc, char *argv[]) {

	int playerCnt = 4;
	string mapFile = "default.map";
	int winLimit = 10;
	bool colour = false;

	for(int i=1;i<argc;i++) {
		if(string(argv[i]) == "-seed") {
			i++;
			srand(stoi(argv[i]));
		} else if(string(argv[i]) == "-map") {
			i++;
			mapFile = argv[i];
		} else if(string(argv[i]) == "-player") {
			i++;
			playerCnt = stoi(argv[i]);
		} else if(string(argv[i]) == "-win") {
			i++;
			winLimit = stoi(argv[i]);
		} else if(string(argv[i]) == "-colour") {
			colour = true;
		}

	}

	bool keepPlaying = true;

	while(keepPlaying) {

		Game game = Game{playerCnt, mapFile, winLimit, colour};

		bool doFirstTurn = true;

		for(int i=1;i<argc;i++) {
			if(string(argv[i]) == "-load") {
				i++;
				string fname{argv[i]};
				LoadGame lg{fname};
				game.accept(&lg);
				doFirstTurn = false;
			} else if(string(argv[i]) == "-board") {
				i++;
				string fname{argv[i]};
				LoadBoard lb{fname};
				game.accept(&lb);
			} 
		}

		if(doFirstTurn) {
			game.firstTurn();
		}

		while(!cin.eof()) {
			if(game.turn()) {
				break;
			}
		}

		if(cin.eof()) {
			break;	
		}

		cout << "Would you like to play again?" << endl << ">";

		string s;
		cin >> s;
		while(s != "yes" && s != "no" && s != "y" && s != "n") {
			cout << ">";
			cin >> s;
		}
		
		if(s == "yes" || s == "y") {
			cout << "Starting again!" << endl;
		} else if(s == "no" || s == "n") {
			cout << "Okay, see you." << endl;
			keepPlaying = false;
		}
	}
	return 0;
}
