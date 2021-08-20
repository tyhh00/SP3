#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H
#include "Weapon.h"
#include "Input.h"

class Flashlight : public Weapon {
public:
	Flashlight();
	~Flashlight();

	void Init();
	void Update(double dt);
	bool IsEqual(Item* item1);
	
	
private:

	bool active;
	float battery;
	float batt_usage_rate;

	float currBatt;
	float maxBatt;

	Input* input;
};


#endif