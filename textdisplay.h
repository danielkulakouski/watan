#ifndef TEXTDISPLAY_H
#define TEXTDISPLAY_H

#include <string>
#include <vector>

#include "observer.h"
#include "state.h"

class TextDisplay: public Observer<DisplayState> {
	private:

		enum ElementType{TEXT, TILEVALUE, TILERESOURCE, GOAL, CRITERIA, GEESE};

		struct TemplateElement {
			ElementType eType;
			std::string value;
			int location;
			TemplateElement(ElementType eType, std::string value, int location):
			eType{eType}, value{value}, location{location} {}
		};

		bool colour;

		std::vector<TemplateElement> mapTemplate;

		std::vector<std::string> tileValues; // %T12%
		std::vector<std::string> tileResources; // %R10------%
		std::vector<std::string> goals; // %G35%
		std::vector<std::string> criteria; // %C25%
		int geese; // %X8----%

		//static const std::vector<std::string> displayColours;

		std::string colourText(std::string text, int clr = 255, std::string extra = "") const;

	public:
		TextDisplay(std::string mapFileName = "default.map", bool colour = false);

		void update(DisplayState &state);

		friend std::ostream &operator<<(std::ostream &out, const TextDisplay &td);
};

#endif
