#include "numeric_range.hpp"



#include <string>
#include <regex>

#include <stdexcept>

NumericRange::NumericRange() {
	low = 0;
	high = 0;
};


NumericRange::NumericRange(LL _from, LL _to) {
	

		if (_from > _to) { low = 0; high = 0; throw std::logic_error("Low bound higher than high bound"); };
		low = _from;
		high = _to;


};

NumericRange::NumericRange(const char* _str_ptr){	

	if (!isStringValidFormat(*_str_ptr)) { throw std::logic_error("Invalid format");return; };

	setLowHighFromValidString(*_str_ptr);

	if (low > high) { low = 0; high = 0; throw std::logic_error("Low bound higher than high bound"); };


};

void NumericRange::setLowHighFromValidString( const char& _str_link ) {
	std::string str(&_str_link);
	std::smatch m;

	std::regex r("\\s*\\[(-?\\d+):(-?\\d+)\\]\\s*");

	std::regex_match(str, m, r);

	low = std::stoll(m[1]);
	high = std::stoll(m[2]);
}



bool NumericRange::isStringValidFormat(const char& _str_link) const {
	std::string str(&_str_link);
	std::regex r("\\s*\\[-?\\d+:-?\\d+\\]\\s*");

	if ( std::regex_match(str,r) ) { 
		return true;
	};

	return false;
};


inline LL NumericRange::getLowBound() const {
	return low;
};


inline LL NumericRange::getHighBound() const {
	return high;
};

LL NumericRange::getWidth() const {
	return high - low + 1;
};


bool NumericRange::contains(LL _num) const {
	return 	( _num >= low && _num <= high );
};

bool NumericRange::intersectsWith(const NumericRange& _range) const {
	return !(_range.getHighBound() < low || _range.getLowBound() > high) ; //not intersects when range1 is less then range2's min or when range1 is bigger then range2's max 
};


bool NumericRange::includes(const NumericRange& _range) const {
	return ( _range.getLowBound() >= low  &&  _range.getHighBound() <= high );
};

bool NumericRange::belongsTo(const NumericRange& _range) const {
	return _range.includes(*this);
};

bool NumericRange::adjacentTo(const NumericRange& _range) const {
	return ( _range.getHighBound() + 1 == this->getLowBound() )|| ( _range.getLowBound() - 1 == this->getHighBound() );
};

bool NumericRange::operator== (const NumericRange& _range) const {
	return (low == _range.getLowBound() &&  high == _range.getHighBound() );
};

bool NumericRange::operator!= (const NumericRange& _range) const {
	return ! (*this == _range);
};

bool NumericRange::operator< (const NumericRange& _range) const {
	return (low < _range.getLowBound()) ? true :  (   (low == _range.getLowBound()) ? (high < _range.getHighBound()) : false   );
};

bool NumericRange::operator<= (const NumericRange& _range) const {
	return *this == _range || *this < _range;
};

bool NumericRange::operator> (const NumericRange& _range) const {
	return _range < *this;
};

bool NumericRange::operator>= (const NumericRange& _range) const {
	return _range <= *this ;
};


NumericRange::Iterator NumericRange::begin() const{
	Iterator it(*this);
	it.setCounter(it.first);
	return it;
};

NumericRange::Iterator NumericRange::end() const {
	Iterator it(*this);
	it.setCounter(it.last + 1);

	return it ;
};

NumericRange::Iterator::Iterator(const NumericRange& _range) {
	first = _range.getLowBound();
	last = _range.getHighBound();
};


void NumericRange::Iterator::setCounter(LL _num){
	counter = _num;
};




LL NumericRange::Iterator::operator*() const {
	return counter;
};

NumericRange::Iterator NumericRange::Iterator::operator++ (int) {

	Iterator copy = *this;

	counter = counter + 1;

	return copy;
};

NumericRange::Iterator& NumericRange::Iterator::operator++ (){
	counter = counter + 1;
	return *this;
};

bool NumericRange::Iterator::operator== (const NumericRange::Iterator &_it) const {
	return this->counter == _it.counter && this->first == _it.first && this->last == _it.last;
};

bool NumericRange::Iterator::operator!= (const NumericRange::Iterator &_it) const {
	return  !( *this == _it );
};


std::ostream & operator << (std::ostream & o, NumericRange _range) {

	o << "[" << _range.getLowBound() << ":"<< _range.getHighBound() << "]";

	return o;
};

