// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _AUTOMATICTEAPOT_HPP_
#define _AUTOMATICTEAPOT_HPP_


#include "teapot.hpp"


class AutomaticTeapot  : public Teapot{
private:
	virtual void noWater() override ;
	virtual void waterBoiling() override;

public:
	AutomaticTeapot(int, int);
	
	AutomaticTeapot(AutomaticTeapot const  &) = delete;
	AutomaticTeapot& operator=(AutomaticTeapot const &) = delete;
	
	AutomaticTeapot(AutomaticTeapot&&) = delete;
	AutomaticTeapot& operator=(AutomaticTeapot&&) = delete;
};


#endif // _AUTOMATICTEAPOT_HPP_
