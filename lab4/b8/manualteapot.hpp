// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _MANUALTEAPOT_HPP_
#define _MANUALTEAPOT_HPP_


#include "teapot.hpp"


class ManualTeapot	:	public Teapot{
private:
	static int const EVAPORATION_SPEED_PERCENTS = 5;
	static int const MINUTES_WITHOUT_WATER_NOT_BURNING = 3;

	int passedWarnings = 0;
	int evaporatedVolume = 0;


	virtual void waterBoiling() override;
	virtual void noWater() override;

public:
	ManualTeapot(int, int) ;

	ManualTeapot(ManualTeapot const  &) = delete;
	ManualTeapot& operator=(ManualTeapot const &) = delete;

	ManualTeapot(ManualTeapot&&) = delete;
	ManualTeapot& operator=(ManualTeapot&&) = delete;

};

#endif // _MANUALTEAPOT_HPP_
