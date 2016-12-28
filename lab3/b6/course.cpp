
#include "course.hpp"
#include "messages.hpp"
#include"checkpoint.hpp"





Course::Course(std::string const  & _crN, ControlForm _ctf): name(_crN),type(_ctf) {
	if (_crN.empty()) { throw std::logic_error(Messages::EmptyCourseName); };
};


void Course::isCheckpointsOK() const {
	if (checkpoints.empty()) { throw std::logic_error(Messages::NoCheckpointsFound); };


	int total = 0;

	for (auto const & pair : checkpoints) {
		total += pair.second->getMaxMark();
		if (total > 100) { throw std::logic_error(Messages::CheckpointsSumMismatch); };
	};

	if(total != 100) { throw std::logic_error(Messages::CheckpointsSumMismatch); };


};


std::string const  & Course::getName() const {
	return name;
};

ControlForm const & Course::getControlForm() const {
	return type;
};


void Course::addCheckpoint(std::unique_ptr<Checkpoint> && _c8t) {
	if (_c8t->getMaxMark() < 1 || _c8t->getMaxMark() > 100) { throw std::logic_error(Messages::InvalidMarkValue); }
	checkpoints.emplace(_c8t->getName(), std::move(_c8t));
};



Checkpoint * Course::getCheckpoint(std::string const  & _c8tName) const {

	try {
		return checkpoints.at(_c8tName).get();
	}catch (std::exception &) {
		throw std::logic_error(Messages::CheckpointCannotBeFound);
	};
};

std::unordered_map<std::string, std::unique_ptr<Checkpoint>> & Course::getCheckpoints() {
	return checkpoints;
};