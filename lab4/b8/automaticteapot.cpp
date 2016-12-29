#include "automaticteapot.hpp"
#include<utility>

AutomaticTeapot::AutomaticTeapot(int _capacity, int _heatSpeed) : Teapot(_capacity, _heatSpeed) {
	iamautomate = true;
};



void AutomaticTeapot::noWater() {
	turnOff();
};


void AutomaticTeapot::waterBoiling() {
	turnOff();
};