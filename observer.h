#ifndef _OBSERVER_H_
#define _OBSERVER_H_

template <typename StateType> class Subject;

template <typename StateType> class Observer {
	public:
		virtual void update(StateType &state) = 0; 
		//virtual ~Observer() = default;
};
#endif
