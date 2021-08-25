#ifndef TRIDENT_H
#define TRIDENT_H
#include "Weapon.h"
#include "Input.h"

class Trident : public Weapon {
public:
	Trident();
	~Trident();

	void Init();
	void Update(double dt);
	bool IsEqual(Item* item1);
	
	bool isWithinLight(Vector3 objPos);
	bool isIntensified();
	
private:

	bool active;
	bool intensified;

	float battery;
	float batt_usage_rate;
	float rate_multiplier;

	float currBatt;
	float maxBatt;

	float light_radius;
	Vector3 light_pos;

	Input* input;
};


#endif