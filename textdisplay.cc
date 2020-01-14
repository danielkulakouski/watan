#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "criteria.h"
#include "goal.h"
#include "tile.h"
#include "player.h"
#include "textdisplay.h"

using namespace std;

void pad(string &padder, string padded, bool right = true) {
	if(padder[0] == '\033') {
		int offset = padder.find_first_of("m");
		padder = padder.substr(offset, padder.size() - 5 - offset);
	}
	if(right) {
		padder = string(padder.length() - padded.length(), ' ') + padded;
	} else {
		padder = padded + string(padder.length() - padded.length(), ' ');
	}
}

string replaceDash(string str) {
	for (int i = 0; i < str.length(); ++i) {
		if (str[i] == '-') {
			str[i] = ' ';
		}
	}

	return str;
}

//const vector<string> TextDisplay::displayColours{"\033[0;31m"};

string TextDisplay::colourText(string text, int clr, string extra) const {
	if(!colour) {
		return text;
	}
	return "\033[" + extra + ";38;5;" + to_string(clr) + "m" + text + "\033[0m"; 
}

TextDisplay::TextDisplay(string mapFileName, bool colour):
colour{colour},
mapTemplate{vector<TemplateElement>{}},
tileValues{vector<string>{}},
tileResources{vector<string>{}},
goals{vector<string>{}},
criteria{vector<string>{}},
geese{-1} {
	ifstream in{mapFileName};

	int tCnt,gCnt,cCnt;
	in >> tCnt >> gCnt >> cCnt;

	for(int i=0;i<tCnt;i++) {
		tileValues.emplace_back(to_string(i));
	}

	for(int i=0;i<tCnt;i++) {
		tileResources.emplace_back(to_string(i));
	}

	for(int i=0;i<gCnt;i++) {
		goals.emplace_back(to_string(i));
	}

	for(int i=0;i<cCnt;i++) {
		criteria.emplace_back(to_string(i));
	}

	string s;
	in >> s;

	while(s != "TEMPLATE") {
		in >> s;
	}

	string line;

	getline(in, line);

	while(line != "END TEMPLATE") {
		
		int prev = -1;

		int cur = line.find("%", prev + 1);

		while(cur != string::npos) {

			mapTemplate.emplace_back(TemplateElement{
				ElementType::TEXT,
				line.substr(prev + 1, cur - prev - 1),
				0
			});

			int next = line.find("%", cur + 1);

			string templateStr = replaceDash(line.substr(cur + 1, next - cur - 1));

			istringstream ss{templateStr};

			char typeC;

			ss >> typeC;

			int loc;

			ss >> loc;

			ElementType eType;

			if(typeC == 'T') {
				eType = ElementType::TILEVALUE;
			} else if(typeC == 'R') {
				eType = ElementType::TILERESOURCE;
			} else if(typeC == 'G') {
				eType = ElementType::GOAL;
			} else if(typeC == 'C') {
				eType = ElementType::CRITERIA;
			} else if(typeC == 'X') {
				eType = ElementType::GEESE;
			}

			string locStr = to_string(loc);

			mapTemplate.emplace_back(TemplateElement{
				eType,
				"",
				loc
			});

			if(typeC == 'T') {
				tileValues[loc] = string(next - cur - 2, ' ');
				pad(tileValues[loc], locStr);
			} else if(typeC == 'R') {
				tileResources[loc] = string(next - cur - 2, ' ');
				pad(tileResources[loc], locStr, false);
			} else if(typeC == 'G') {
				goals[loc] = string(next - cur - 2, ' ');
				pad(goals[loc], locStr);
			} else if(typeC == 'C') {
				criteria[loc] = string(next - cur - 2, ' ');
				pad(criteria[loc], locStr);
			} 

			prev = next;
			cur = line.find("%", prev + 1);
		}

		mapTemplate.emplace_back(TemplateElement{
			ElementType::TEXT,
			line.substr(prev + 1) + "\n",
			0
		});

		getline(in, line);
	}
	
	in.close();
}

void TextDisplay::update(DisplayState &state) {
	if(state.sType == SubjectType::CRITERIA) {

		pad(criteria[state.location], ((Criteria*)state.subject)->toStr());
		criteria[state.location] = colourText(criteria[state.location], ((Criteria*)state.subject)->getPlayer()->getColourId(), "1");

	} else if(state.sType == SubjectType::GOAL) {

		pad(goals[state.location], ((Goal*)state.subject)->toStr());
		goals[state.location] = colourText(goals[state.location], ((Goal*)state.subject)->getColourId(), "1");

	} else if(state.sType == SubjectType::TILE) {

		pad(tileValues[state.location], ((Tile*)state.subject)->valueStr());

		int valueClr = 255;
		if(((Tile*)state.subject)->getValue() == 6 || ((Tile*)state.subject)->getValue() == 8) {
			valueClr = 196;	
		}

		tileValues[state.location] = colourText(tileValues[state.location], valueClr);

		int resourceClr = ((Tile*)state.subject)->getResources().getColourId();

		pad(tileResources[state.location], ((Tile*)state.subject)->getResourcesType(), false);
		tileResources[state.location] = colourText(tileResources[state.location], resourceClr, "3");

	} else if(state.sType == SubjectType::GEESE) {

		geese = state.location;

	}
}

std::ostream &operator<<(std::ostream &out, const TextDisplay &td) {
	for(auto &element: td.mapTemplate) {
		if(element.eType == TextDisplay::ElementType::TEXT) {
			out << element.value;
		} else if(element.eType == TextDisplay::ElementType::GEESE) {
			if(element.location == td.geese) {
				out << td.colourText("G", 252, "1") << td.colourText("E", 240, "1");
				out << td.colourText("E", 252, "1") << td.colourText("S", 240, "1");
				out << td.colourText("E", 252, "1");
			} else {
				out << "     ";
			}
		} else if(element.eType == TextDisplay::ElementType::TILEVALUE) {
			out << td.tileValues[element.location];
		} else if(element.eType == TextDisplay::ElementType::TILERESOURCE) {
			out << td.tileResources[element.location];
		} else if(element.eType == TextDisplay::ElementType::GOAL) {
			out << td.goals[element.location];
		} else if(element.eType == TextDisplay::ElementType::CRITERIA) {
			out << td.criteria[element.location];
		}
	}
	return out;
}
