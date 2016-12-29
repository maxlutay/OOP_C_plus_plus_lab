
#include "controller.hpp"
#include "messages.hpp"





Teapot * Controller::getTeapot(int _teapotIndex) const {
	try {
		return teapots.at(_teapotIndex).get();
	}
	catch (std::out_of_range & err) {//if not exist
		throw std::logic_error(Messages::WrongTeapotIndex);
	};
};






void Controller::createManualTeapot(int _maxVolume, int _speed)  {
	teapots.push_back(std::make_unique<ManualTeapot>(_maxVolume,_speed));
};



void Controller::createAutomaticTeapot(int _maxVolume, int _speed) {
	teapots.push_back(std::make_unique<AutomaticTeapot>(_maxVolume, _speed));
};

 

int Controller::getTeapotCurrentVolume(int _teapotIndex) const {
		return getTeapot(_teapotIndex)->getCurrentVolume();
};

int Controller::getTeapotMaxVolume(int _teapotIndex) const {
	return getTeapot(_teapotIndex)->getCapacity();
};

int Controller::getTeapotCurrentTemperature(int _teapotIndex) const {
	return getTeapot(_teapotIndex)->getCurrentTemperature();
};

int Controller::getTeapotBoilingSpeed(int _teapotIndex) const {
		return getTeapot(_teapotIndex)->getHeatSpeed();
};

 

bool Controller::isTeapotOn(int _teapotIndex) const {
		return getTeapot(_teapotIndex)->getPowerStatus();
};

void Controller::turnOnTeapot(int _teapotIndex) {
	getTeapot(_teapotIndex)->turnOn();
};

void Controller::turnOffTeapot(int _teapotIndex) {
	getTeapot(_teapotIndex)->turnOff();
};


void Controller::addWater(int _teapotIndex, int _volume) {
	if (_volume < 0) { throw std::logic_error(Messages::NonPositiveVolume); };
	getTeapot(_teapotIndex)->pourIn(_volume);

};

void Controller::takeWater(int _teapotIndex, int _volume) {
	if (_volume < 0) { throw std::logic_error(Messages::NonPositiveVolume); };
	getTeapot(_teapotIndex)->pourOut(_volume);

};

 

void Controller::wait(int _minutesCount) {
	bool once = _minutesCount == 1;
	for (auto const & teapot: teapots) {
		once ? teapot->refreshState() : teapot->refreshState(_minutesCount);
	};
};