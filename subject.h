#ifndef SUBJECT_H
#define SUBJECT_H
#include <vector>
#include <iostream>

template <typename StateType> class Observer;

template <typename StateType> class Subject {
	private:
		std::vector<Observer<StateType>*> observers;

	public:
		void attach(Observer<StateType> *o);  
		void notify(StateType state);
};

template <typename StateType>
void Subject<StateType>::attach(Observer<StateType> *o) {
	observers.emplace_back(o);
}

template <typename StateType>
void Subject<StateType>::notify(StateType state) {
	for (auto &ob : observers) {
		ob->update(state);
	}
}

#endif
