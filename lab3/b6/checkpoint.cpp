// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "checkpoint.hpp"
#include "messages.hpp"

/*****************************************************************************/

Checkpoint::Checkpoint(std::string const  & _n2e, int _m) : name(_n2e) , maxmark(_m) {
	if (_n2e.empty()) { throw std::logic_error(Messages::EmptyCheckpointName); };
	if (maxmark < 0 || maxmark > 100) { throw std::logic_error(Messages::InvalidMarkValue); };
};


std::string const  & Checkpoint::getName() const {
	return name;
};

int const & Checkpoint::getMaxMark() const {
	return maxmark;
};

/*****************************************************************************/ 