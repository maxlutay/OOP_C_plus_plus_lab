// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _STUDENT_HPP_
#define _STUDENT_HPP_


#include<string>
#include<unordered_map>


class Checkpoint;



class Student
{

public:


	Student(std::string const  &);

	Student(Student const  &) = delete;
	Student& operator =(Student const  &) = delete;

	Student(Student &&) = default;
	Student& operator =(Student &&) = default;



	~Student() = default;


	std::string const  & getFullName() const;

	int getCheckpointMark(Checkpoint  *) const;
	void setCheckpointMark(Checkpoint  *, int);

	std::unordered_map<Checkpoint  *, int> & getMarksset();

private:


	std::string const fullname;
	std::unordered_map<Checkpoint  *, int> marksset{};


 
};

#endif // _STUDENT_HPP_
