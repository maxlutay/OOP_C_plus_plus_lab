#include "student.hpp"
#include "checkpoint.hpp"
#include "messages.hpp"



Student::Student(std::string const  & _s5tFullName):fullname(_s5tFullName) {
	if (_s5tFullName.empty()) { throw std::logic_error(Messages::EmptyStudentName); }
};



std::string const  & Student::getFullName() const {
	return fullname;
};

int Student::getCheckpointMark(Checkpoint   * _c8t) const {
	try {
		return marksset.at(_c8t) ;
	}catch (std::exception & err) {
		return 0;
		//throw std::logic_error(Messages::CheckpointCannotBeFound);
	};

};


void Student::setCheckpointMark(Checkpoint * _c8t, int _m) {
	if (_m < 0 || _m > _c8t->getMaxMark()) { throw std::logic_error(Messages::InvalidMarkValue);};
	//marksset.emplace(_c8t, _m );
	marksset[_c8t] = _m;
		
	
}



std::unordered_map<Checkpoint  *, int> &Student::getMarksset() {
	return marksset;
};