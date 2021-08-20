#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H
#include "Weapon.h"
#include "Input.h"

class Flashlight : public Weapon{
public:
	Flashlight(int _quantity = 1, int stemLength = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;
	
private:
	int stemLength;

	Input* input;
};


#endif