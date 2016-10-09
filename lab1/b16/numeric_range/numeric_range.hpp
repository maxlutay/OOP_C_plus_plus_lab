#ifndef _NUMERIC_RANGE_HPP_
#define _NUMERIC_RANGE_HPP_


#include <iostream>
#include <string>
#include <regex>

typedef long long LL;



class NumericRange{
	LL low, high;

	bool isStringValidFormat(const char*) const;

	LL getLowFromStr(const char*) const;
	LL getHighFromStr(const char*) const;



public:
	class Iterator {
		LL first;
		LL last;
		LL counter;


		void setCounter(LL _num);


		friend class NumericRange;

	  public:
		Iterator(NumericRange);

		bool operator== (Iterator) const;
		bool operator!= (Iterator) const;
		Iterator&  operator ++();
		Iterator operator ++(int);



		LL operator* () const;// read only
		

	} ;

 public:

	NumericRange();
	NumericRange(LL, LL);
	NumericRange(const char*);
	inline LL getLowBound() const;
	inline LL getHighBound() const;
	LL getWidth() const;
	bool contains(LL) const;
	bool intersectsWith(NumericRange) const;
	bool includes(NumericRange) const;
	bool belongsTo(NumericRange) const;
	bool adjacentTo(NumericRange) const;


	bool operator!= (NumericRange) const;
	inline bool operator== (NumericRange) const;
	bool operator< (NumericRange) const;
	bool operator> (NumericRange) const;
	bool operator<= (NumericRange) const;
	bool operator>= (NumericRange)  const;

	Iterator begin() const;
	Iterator end() const;

};

std::ostream & operator << (std::ostream &, NumericRange);

#endif //  _NUMERIC_RANGE_HPP_
