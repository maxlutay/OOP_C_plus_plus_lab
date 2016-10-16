#ifndef _NUMERIC_RANGE_HPP_
#define _NUMERIC_RANGE_HPP_


#include <iostream>

typedef long long LL;



class NumericRange{


private:
	LL low, high;

	bool isStringValidFormat(const char&) const;

	void setLowHighFromValidString(const char&);


public:

	class Iterator {
		LL first;
		LL last;
		LL counter;


		void setCounter(LL _num);


		friend class NumericRange;

	  public:
		Iterator(const NumericRange&);

		bool operator== (const Iterator&) const;
		bool operator!= (const Iterator&) const;
		Iterator&  operator ++();
		Iterator operator ++(int);



		LL operator* () const;// read only
		

	} ;



	NumericRange();
	NumericRange(LL, LL);
	NumericRange(const char*);
	inline LL getLowBound() const;
	inline LL getHighBound() const;
	LL getWidth() const;
	bool contains(LL) const;
	bool intersectsWith(const NumericRange&) const;
	bool includes(const NumericRange&) const;
	bool belongsTo(const NumericRange&) const;
	bool adjacentTo(const NumericRange&) const;


	bool operator!= (const NumericRange&) const;
	inline bool operator== (const NumericRange&) const;
	bool operator< (const NumericRange&) const;
	bool operator> (const NumericRange&) const;
	bool operator<= (const NumericRange&) const;
	bool operator>= (const NumericRange&)  const;

	Iterator begin() const;
	Iterator end() const;

};

std::ostream & operator << (std::ostream &, NumericRange);

#endif //  _NUMERIC_RANGE_HPP_
