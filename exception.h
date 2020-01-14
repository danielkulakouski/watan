#include <stdexcept>

class NotEnoughResourcesException: public std::logic_error {
	public: 
		NotEnoughResourcesException(const char* what_arg): std::logic_error{what_arg} {}
};

class AlreadyAchievedException: public std::logic_error {
	public: 
		AlreadyAchievedException(const char* what_arg): std::logic_error{what_arg} {}
};

class NotConnectedException: public std::logic_error {
	public: 
		NotConnectedException(const char* what_arg): std::logic_error{what_arg} {}
};

class AlreadyMaxedException: public std::logic_error {
	public: 
		AlreadyMaxedException(const char* what_arg): std::logic_error{what_arg} {}
};

class TooCloseException: public std::logic_error {
	public: 
		TooCloseException(const char* what_arg): std::logic_error{what_arg} {}
};

class AlreadyCompletedException: public std::logic_error {
	public: 
		AlreadyCompletedException(const char* what_arg): std::logic_error{what_arg} {}
};

class WrongPlayerException: public std::logic_error {
	public: 
		WrongPlayerException(const char* what_arg): std::logic_error{what_arg} {}
};
