#ifndef RESOURCES_H
#define RESOURCES_H

#include <unordered_map> 
#include <vector>
#include <list>
#include <string>

#include "constants.h"

class Goal;

class Resources {
	private:
		std::unordered_map<std::string, int> resource;

	public:

		static const std::vector<std::string> ResourceTypes;
		static const std::vector<int> ResourceColoursIds;

		Resources();
		Resources(std::vector<int>);
		Resources(std::string type, int value=1);
		Resources(CompletionLevel);
		Resources(Goal&);

		std::string getResourcesType();
		int getResourcesId();
		int getColourId();
		std::string toStr(std::string sep = " ", bool doEmpty = true, bool doType = true);

		int total();

		Resources discard(int);

		Resources operator+(const Resources &rhs) const;
		Resources operator-(const Resources &rhs) const;
		Resources operator*(const int &rhs) const;
		Resources operator/(const int &rhs) const;

		bool operator==(const Resources &rhs) const;
		bool operator!=(const Resources &rhs) const;
		bool operator<=(const Resources &rhs) const;
		bool operator>=(const Resources &rhs) const;

		static bool isType(std::string);
};

#endif

