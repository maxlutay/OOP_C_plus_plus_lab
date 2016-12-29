#include "teapot.hpp"
#include"messages.hpp"

#include<stdexcept>

Teapot::Teapot(int _capacity, int _heatSpeed) :  capacity(_capacity),heatSpeed(_heatSpeed) {
	if (_capacity <= 0) { throw std::logic_error(Messages::NonPositiveVolume); };
	if (_heatSpeed <= 0) { throw std::logic_error(Messages::NonPositiveSpeed); };


};




bool Teapot::turnOn() {
	bool ret = powerStatus;
	powerStatus = true;
	return ret;
};

bool Teapot::turnOff() {
	bool ret = powerStatus;
	powerStatus = false;
	return ret;
};


void Teapot::refreshState() {
//if on
	
	
	if (powerStatus) {
		if (!currentVolume) { noWater();return; };


		if ( currentTemperature  < 100) {currentTemperature += heatSpeed;
		//985	return;

		};
		//334	if (currentTemperature >= 100) { waterBoiling(); };
		//985		waterBoiling();	
		return;

	};



//if off
	

	if (currentTemperature > STANDART_TEMPERATURE) {
	
		////temperature sensor sensitivity min is STANDART_TEMPERATURE
		//if (currentTemperature - COOLING_SPEED < STANDART_TEMPERATURE) { currentTemperature = STANDART_TEMPERATURE; return; };
		//currentTemperature -= COOLING_SPEED;return;
		currentTemperature -= COOLING_SPEED;return;


	};


	return;
};



void Teapot::refreshState(int _times) {
	for (int i = 0; i < _times; i++) {
		refreshState();
	};
};




void Teapot::pourIn(int _volume) {
	if (powerStatus == true) { throw std::logic_error(Messages::TeapotIsNotModifiable); };
	if (_volume + currentVolume > capacity) {throw std::logic_error(Messages::TeapotVolumeOverflow);};
	
	if (tinyfirstpourinmem < 0) { tinyfirstpourinmem = _volume; };
	
	currentVolume += _volume;
};


void Teapot::pourOut(int _volume) {
	if (powerStatus == true) { throw std::logic_error(Messages::TeapotIsNotModifiable); };
	if (currentVolume - _volume < 0) { throw std::logic_error(Messages::TeapotVolumeUnderflow); };
	currentVolume -= _volume;
};

int Teapot::getCurrentVolume() const {
	return currentVolume;
};

int Teapot::getCapacity() const {
	return capacity;
};

int Teapot::getCurrentTemperature() const {
	return currentTemperature;
};


int Teapot::getHeatSpeed() const {
	return heatSpeed;
};

bool Teapot::getPowerStatus() const {
	return powerStatus;
};