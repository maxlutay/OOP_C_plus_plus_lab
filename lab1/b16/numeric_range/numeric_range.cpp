#include "numeric_range.hpp"

#include <stdexcept>

NumericRange::NumericRange() {
	low = 0;
	high = 0;
};


NumericRange::NumericRange(LL _from, LL _to) {
	
	/*try {*/
		if (_from > _to) { throw std::logic_error("Low bound higher than high bound"); };
		low = _from;
		high = _to;
	/*}catch (std::logic_error err) {
		std::cerr<< err.what() ;
	};*/

};

NumericRange::NumericRange(const char* _str_ptr){	
	/*try {*/
	if (!isStringValidFormat(_str_ptr)) { std::cout << "\nInvalid format\n"; throw std::logic_error("Invalid format");return; };
		low = getLowFromStr(_str_ptr);
		high = getHighFromStr(_str_ptr);
		std::cout << "\n low : " << low << "  high : " << high << "\n";
		if (low > high) { low = 0; high = 0; throw std::logic_error("Low bound higher than high bound"); };

	/*}catch (std::logic_error err) {
		std::cerr << err.what();
		
	};*/
};

LL NumericRange::getLowFromStr(const char* _str_ptr) const {
	std::string str(_str_ptr);
	//std::smatch m;
	//std::regex("(\\d+)");
	//std::cout << str << "...";
	//for (auto v : m) {
	//	std::cout << v << " ... ";
	//};
	//return std::stoll( m[0].str() );
	int from = str.find_first_of('[') + 1;
	std::string t = str.substr( from , str.find_first_of(':') - from );
	std::cout << t << "__/";
	return std::stoll( t );
};

LL NumericRange::getHighFromStr(const char* _str_ptr) const {
	std::string str(_str_ptr);
	//std::smatch m;
	//std::regex("(\\d+)");
	//std::cout << str << "...";
	//for (auto v : m) {
	//	std::cout << v << " ... ";
	//};
	//return std::stoll( m[1].str() );

	int from = str.find_first_of(':') + 1;
	int len = str.size() - from - 1;

	std::string t = str.substr(from, len  );
	std::cout << t << "__/";
	return std::stoll(t);

};

bool NumericRange::isStringValidFormat(const char* _str_ptr) const {
	std::string str(_str_ptr);
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

bool NumericRange::intersectsWith(NumericRange _range) const {
	return !(_range.getHighBound() < low || _range.getLowBound() > high) ; //not intersects when range1 is less then range2's min or when range1 is bigger then range2's max 
};


bool NumericRange::includes(NumericRange _range) const {
	return ( _range.getLowBound() >= low  &&  _range.getHighBound() <= high );
};

bool NumericRange::belongsTo(NumericRange _range) const {
	return _range.includes(*this);
};

bool NumericRange::adjacentTo(NumericRange _range) const {
	return _range.getHighBound() < this->getLowBound() || _range.getLowBound() > this->getHighBound();
};

bool NumericRange::operator== (NumericRange _range) const {
	return (low == _range.getLowBound() &&  high == _range.getHighBound() );
};

bool NumericRange::operator!= (NumericRange _range) const {
	return ! (*this == _range);
};

bool NumericRange::operator< (NumericRange _range) const {
	return (low < _range.getLowBound()) ? true :  (   (low == _range.getLowBound()) ? (high < _range.getHighBound()) : false   );
};

bool NumericRange::operator<= (NumericRange _range) const {
	return *this == _range || *this < _range;
};

bool NumericRange::operator> (NumericRange _range) const {
	return _range < *this;
};

bool NumericRange::operator>= (NumericRange _range) const {
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

NumericRange::Iterator::Iterator(NumericRange _range) {
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
	counter = counter + 1;
	return *this;
};

NumericRange::Iterator& NumericRange::Iterator::operator++ (){
	counter = counter + 1;
	return *this;
};

bool NumericRange::Iterator::operator== (NumericRange::Iterator _it) const {
	return this->counter == _it.counter && this->first == _it.first && this->last == _it.last;
};

bool NumericRange::Iterator::operator!= (NumericRange::Iterator _it) const {
	return  !( *this == _it );
};


std::ostream & operator << (std::ostream & o, NumericRange _range) {
	//for (LL i = _range.getLowBound(), l = _range.getHighBound(); i < l; i += 1) {
	//	o << i ;
	//};

	o << "[" << _range.getLowBound() << ":"<< _range.getHighBound() << "]";

	return o;
};

