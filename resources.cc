#include <sstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <algorithm>

#include "resources.h"

using namespace std;

const vector<string> Resources::ResourceTypes{"CAFFEINE", "LAB", "LECTURE", "STUDY", "TUTORIAL", "NETFLIX"};
const vector<int> Resources::ResourceColoursIds{193, 120, 229, 248, 223, 210};

Resources::Resources() {
	for(int i=0;i<ResourceTypes.size();i++) {
		resource[ResourceTypes[i]] = 0;
	}
}

Resources::Resources(vector<int> lst) {
	for(int i=0;i<ResourceTypes.size();i++) {
		if(i < lst.size()) {
			resource[ResourceTypes[i]] = lst[i];
		} else {
			resource[ResourceTypes[i]] = 0;
		}
	}
}

Resources::Resources(string type, int value):
Resources{} {
	resource[type] = value;
}

Resources::Resources(CompletionLevel lvl):
Resources{} {
	if(lvl == CompletionLevel::NOT) {
		resource["CAFFEINE"] = 1;
		resource["LAB"] = 1;
		resource["LECTURE"] = 1;
		resource["TUTORIAL"] = 1;
	} else if(lvl == CompletionLevel::ASSIGNMENT) {
		resource["LECTURE"] = 2;
		resource["STUDY"] = 3;
	} else if(lvl == CompletionLevel::MIDTERM) {
		resource["CAFFEINE"] = 3;
		resource["LAB"] = 2;
		resource["LECTURE"] = 2;
		resource["TUTORIAL"] = 1;
		resource["STUDY"] = 2;
	}
}

Resources::Resources(Goal& goal):
Resources{} {
	resource["TUTORIAL"] = 1;
	resource["STUDY"] = 1;
}

string Resources::getResourcesType() {
	
	for(auto rType: ResourceTypes) {
		
		if(resource[rType] > 0) {
			return rType;
		}
	}

	return "NETFLIX";
}

int Resources::getResourcesId() {
	
	for(int i=0;i<ResourceTypes.size();i++) {
		auto rType = ResourceTypes[i];
		
		if(resource[rType] > 0) {
			return i;
		}
	}

	return ResourceTypes.size() - 1;
}

int Resources::getColourId() {
	return ResourceColoursIds[getResourcesId()];
}

string Resources::toStr(std::string sep, bool doEmpty, bool doType) {
	
	ostringstream out{};

	for(auto type: ResourceTypes) {
		if(type != "NETFLIX") {
			if(resource[type] > 0 || doEmpty) {
				out << resource[type];
				if(doType) {
					out << " " << type;
				}
				out << sep;
			}
		}
	}

	string res = out.str();

	if(res.length() > 0) {
		return res.substr(0, res.length()-sep.length());
	}
	return res;
}

int Resources::total() {
	int cnt = 0;
	for(auto type: ResourceTypes) {
		cnt += resource[type];
	}
	return cnt;
}

Resources Resources::discard(int cnt) {
	Resources discarded{};

	int tot = total();
	
	for(int i = tot; i > tot - cnt; i--) {
		int r = rand() % i;
		for(auto type: ResourceTypes) {
			if(r < resource[type]) {
				resource[type]--;
				discarded = discarded + Resources{type};
				break;
			} else {
				r -= resource[type];
			}
		}
	}

	return discarded;
}

Resources Resources::operator+(const Resources &rhs) const {
	Resources res{};
	for(auto type: ResourceTypes) {
		res.resource[type] = resource.at(type) + rhs.resource.at(type);
	}
	return res;
}

Resources Resources::operator-(const Resources &rhs) const {
	Resources res{};
	for(auto type: ResourceTypes) {
		res.resource[type] = resource.at(type) - rhs.resource.at(type);
	}
	return res;
}

Resources Resources::operator*(const int &rhs) const {
	Resources res{};
	for(auto type: ResourceTypes) {
		res.resource[type] = resource.at(type) * rhs;
	}
	return res;
}

Resources Resources::operator/(const int &rhs) const {
	Resources res{};
	for(auto type: ResourceTypes) {
		res.resource[type] = resource.at(type) / rhs;
	}
	return res;
}

bool Resources::operator==(const Resources &rhs) const {
	for(auto type: ResourceTypes) {
		if(resource.at(type) != rhs.resource.at(type)) {
			return false;
		}
	}
	return true;
}

bool Resources::operator!=(const Resources &rhs) const {
	return !(*this == rhs);
}

bool Resources::operator<=(const Resources &rhs) const {
	for(auto type: ResourceTypes) {
		if(resource.at(type) > rhs.resource.at(type)) {
			return false;
		}
	}
	return true;
}

bool Resources::operator>=(const Resources &rhs) const {
	return rhs <= *this;
}

bool Resources::isType(string maybeType) {
	return find(ResourceTypes.begin(), ResourceTypes.end(), maybeType) != ResourceTypes.end();
}
