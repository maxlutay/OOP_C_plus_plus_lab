// (C) 2013-2016, Sergei Zaychenko, KNURE, Kharkiv, Ukraine

#ifndef _TEAPOT_HPP_
#define _TEAPOT_HPP_


class Teapot{
private:

	static int const STANDART_TEMPERATURE = 20;
	static int const COOLING_SPEED = 2;


	

	int const capacity;
	int const heatSpeed;
	


	int currentTemperature = STANDART_TEMPERATURE;
	bool powerStatus = false;


	virtual void waterBoiling() = 0;
	virtual void noWater() = 0;



protected:

	int currentVolume = 0;

	int tinyfirstpourinmem = -1;

	Teapot(int,int);

	Teapot(Teapot const &) = delete;
	Teapot& operator = (Teapot const  &) = delete;

	Teapot(Teapot &&) = delete;
	Teapot& operator = (Teapot &&) = delete;



public:

	
	bool turnOn();//returns previous state
	bool turnOff();//returns previous state

	void pourIn(int);


	void pourOut(int);


	int getCurrentVolume() const;
	int getCapacity() const;
	int getCurrentTemperature() const;
	int getHeatSpeed() const;
	bool getPowerStatus() const;

	void refreshState();
	void refreshState(int);

	virtual ~Teapot() = default;
};


#endif // _TEAPOT_HPP_
