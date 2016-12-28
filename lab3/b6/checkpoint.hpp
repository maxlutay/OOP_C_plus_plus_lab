// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _CHECKPOINT_HPP_
#define _CHECKPOINT_HPP_


#include<string>


class Checkpoint{
public:
	
	Checkpoint(std::string const &, int);
	std::string const & getName() const;
	int const & getMaxMark() const;

private:

	std::string const name;
	int const maxmark;

};

#endif // _CHECKPOINT_HPP_
