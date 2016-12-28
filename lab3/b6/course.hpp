// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _COURSE_HPP_
#define _COURSE_HPP_

/*****************************************************************************/

#include<string>
#include<unordered_map>
#include<memory>
#include"controlform.hpp"

class Checkpoint;



class Course{

public:	
	Course(std::string const  &, ControlForm);


	Course(Course const  &) = delete;
	Course& operator =(Course const  &) = delete;

	Course(Course &&) = default;
	Course& operator =(Course &&) = default;

	~Course() = default;

	std::string const  & getName() const;

	void isCheckpointsOK() const;

	void addCheckpoint(std::unique_ptr<Checkpoint> && _c8t);

	Checkpoint * getCheckpoint(std::string const & _c8tName) const;

	ControlForm const  & getControlForm() const;


	std::unordered_map<std::string, std::unique_ptr<Checkpoint>> & getCheckpoints();


private:
	std::string const name;
	ControlForm const type;
	std::unordered_map<std::string ,std::unique_ptr<Checkpoint>> checkpoints;





};



#endif // _COURSE_HPP_
