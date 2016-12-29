#include "manualteapot.hpp"
#include "messages.hpp"

#include<stdexcept>

ManualTeapot::ManualTeapot(int _capacity, int _heatSpeed) : Teapot(_capacity, _heatSpeed) {};



void ManualTeapot::waterBoiling() {

	////maybe sth wrong with tests on 370

	//evaporatedVolume = currentVolume * 10 *EVAPORATION_SPEED_PERCENTS;//percents * 0.01 * 1000 = percents * 10
	//currentVolume -= evaporatedVolume/1000;	

		evaporatedVolume = tinyfirstpourinmem * 10 * EVAPORATION_SPEED_PERCENTS;
		currentVolume -= evaporatedVolume / 1000;


};





void ManualTeapot::noWater() {
	++passedWarnings;
	if (passedWarnings == MINUTES_WITHOUT_WATER_NOT_BURNING) {
		throw std::logic_error(Messages::TeapotAlmostBurned);
	};
	
};


