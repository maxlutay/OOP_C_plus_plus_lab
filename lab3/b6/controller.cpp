// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#include "controller.hpp"
#include"course.hpp"
#include"student.hpp"
#include"checkpoint.hpp"

#include "messages.hpp"


#include<map>
#include<algorithm>

/*****************************************************************************/

Course * Controller::getCourse(std::string const  & _c4eName) const {

	try {
		return courses.at(_c4eName).get();
	}catch (std::exception &) {
		throw std::logic_error(Messages::CourseCannotBeFound);
	};
};


Student * Controller::getStudent(std::string const  &  _s5tFullName ) const {
	try {
		return students.at(_s5tFullName).get();
	}catch (std::exception &) {
		throw std::logic_error(Messages::StudentCannotBeFound);
	};
};

void Controller::addCourse(std::string const  & _c4eName, ControlForm _c5lForm) {
	try {
		courses.at(_c4eName);
	}catch (std::exception & ) {
		courses.emplace(_c4eName,std::make_unique<Course>(_c4eName,_c5lForm) );
		return;
	};
	throw std::logic_error(Messages::DuplicateCourse); 		
};

ControlForm Controller::getCourseControlForm(std::string const  & _courseName) const {
	try {
		return getCourse(_courseName)->getControlForm();		
	}catch (std::exception & ) {
		throw;
	};
};



void Controller::validateCourseCheckpoints(std::string const  & _coursename) const {
	try {
		getCourse(_coursename)->isCheckpointsOK();

	}catch (std::exception & ) {
		throw;
	};
};

void Controller::addCheckpoint(std::string const  & _c4eName, std::string const  & _c8tName, int _mxMark) {
	try {
		getCourse(_c4eName)->addCheckpoint(std::make_unique<Checkpoint>(_c8tName, _mxMark));

	}catch (std::exception & ) {
		throw;
	};
};


int Controller::getCheckpointMaxMark(std::string const  & _c4eName, std::string const  & _c8tName) const {
	try{
		return getCourse(_c4eName)->getCheckpoint(_c8tName)->getMaxMark();


	}catch (std::exception & ) {
		throw;
	};
};


int Controller::getCheckpointMinMark(std::string const  & _c4eName, std::string const  & _c8tName) const {
	try {

		return getCheckpointMaxMark(_c4eName, _c8tName) * 0.6;


	}catch (std::exception & ) {
		throw;
	};
};


void Controller::addStudent(std::string const  & _s5tFullName) {
	try {
		students.at(_s5tFullName);
	}catch (std::logic_error & ) {
		students.emplace(_s5tFullName,std::make_unique<Student>(_s5tFullName) );
		return;
	};
	throw std::logic_error(Messages::DuplicateStudent);

};



void Controller::setStudentCheckpointResult(const std::string & _fullName,
											const std::string & _courseName,
											const std::string & _checkpointName,
											int _mark
){

	try {
		getStudent(_fullName)->setCheckpointMark(getCourse(_courseName)->getCheckpoint(_checkpointName), _mark);
	}catch (std::exception &) {
		throw;
	};

};



void Controller::addStudentCheckpoint(
	const std::string & _fullName
	, const std::string & _courseName
	, const std::string & _checkpointName
	, int _mark
) {
	try {
		setStudentCheckpointResult(_fullName, _courseName, _checkpointName, _mark);
	}catch (std::exception &) {
		throw;
	};
};

int Controller::getStudentCheckpointMark(
	const std::string & _fullName
	, const std::string & _courseName
	, const std::string & _checkpointName
) {
	try {
		return getStudent(_fullName)->getCheckpointMark(getCourse(_courseName)->getCheckpoint(_checkpointName));
	}catch (std::exception &) {
		throw;
	};
};

/*-----------------------------------------------------------------*/
std::vector< std::string > Controller::getStudentsPermittedForExams() const { 
	std::vector< std::string > ret;


	int cn = courses.size();
	int n ;

	for (auto const & studentpair : students) {

		n = cn;

 
		for (auto const & coursepair : courses) {

			int min = 60;

			for (auto  & checkpair : coursepair.second->getCheckpoints() ) {
				

				min-=studentpair.second->getCheckpointMark(checkpair.second.get());

			};

			if (min <= 0) { n--; };

		};
		
		if (n <= 0) { ret.push_back(studentpair.second->getFullName()); };

	};



	std::sort(ret.begin(), ret.end());

	return ret;
};

std::vector< std::string > Controller::getCoursesWithEveryonePassed() const {
	std::vector< std::string > ret;


	int cn = students.size();
	int n;
	for (auto const & coursepair : courses) {
		n = cn;


		for (auto const & studentpair : students) {

		

			int min = 60;

			for (auto & checkpair : coursepair.second->getCheckpoints()) {


				min -= studentpair.second->getCheckpointMark(checkpair.second.get());

			};

			if (min <= 0) { n--; };

		};

		if (n <= 0) { ret.push_back(coursepair.second->getName()); };

	};



	std::sort(ret.begin(), ret.end());


	return ret;
};

#include<iostream>


std::vector< std::pair< std::string, double > > Controller::getAverageScoreByCourse() const {
	std::vector< std::pair< std::string, double > > ret;




	for (auto & coursepair : courses) {

		
		int coursetrunc = 0;

		for (auto const & studentpair : students) {


			int trunc = 0;

			for (auto & checkpair : coursepair.second->getCheckpoints()) {
				trunc += studentpair.second->getCheckpointMark(checkpair.second.get() ) ;
			};



			coursetrunc += trunc;
		};
		

		ret.push_back(std::make_pair(coursepair.second->getName(), static_cast<double>(coursetrunc)/students.size()   ) );

	};
	
	std::sort(ret.begin(), ret.end(), [](std::pair<std::string, double> __a, std::pair<std::string, double> __b )->bool { 
		return __a.first < __b.first;
	});

	return ret;
};


std::string Controller::getStudentWithBestAverageScore() const {
	

	std::vector<std::pair<std::string,double> > bestStudents;
	

	

	for (auto const & studentpair : students) {

		int allcourses = 0;

		for (auto & coursepair : courses) {
			int onecourse = 0 ;
			for (auto & checkpair : coursepair.second->getCheckpoints()) {

				onecourse += studentpair.second->getCheckpointMark(checkpair.second.get());

			};
			allcourses += onecourse;
		};


		double aver = static_cast<double>(allcourses) / students.size();

		if (bestStudents.empty()) { bestStudents.push_back(std::make_pair(studentpair.first, aver));
		}else if (bestStudents[0].second < aver) {
			bestStudents.clear();
			bestStudents.push_back(std::make_pair(studentpair.second->getFullName(),aver ));
		}else if (bestStudents[0].second == aver) {
			bestStudents.push_back(std::make_pair(studentpair.second->getFullName(), aver));
		};


	};


	std::sort(bestStudents.begin(), bestStudents.end(), [](std::pair<std::string,double> __a, std::pair<std::string, double> __b)->bool {
		return __a.first < __b.first;
	});


	return bestStudents[0].first;
};


