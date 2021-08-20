#ifndef FLASHLIGHT_H
#define FLASHLIGHT_H
#include "Weapon.h"
#include "Keyboard.h"

class Flashlight : public Weapon {
public:
	Flashlight();
	~Flashlight();

	void Init();
	void Update(double dt);
	bool IsEqual(Item* item1);
	
private:
	Keyboard* keyboard;
};


#endif