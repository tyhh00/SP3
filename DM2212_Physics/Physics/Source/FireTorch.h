#ifndef FIRETORCH_H
#define FIRETORCH_H
#include "Weapon.h"
#include "Input.h"

class FireTorch : public Weapon {
public:
	FireTorch();
	~FireTorch();

	void Init();
	void Update(double dt);
	bool IsEqual(Item* item1);
	
private:

	Input* input;
};


#endif