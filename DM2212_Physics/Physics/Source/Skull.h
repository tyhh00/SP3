#ifndef SKULL_H
#define SKULL_H
#include "Consumable.h"
#include "Input.h"

class Skull : public Consumable {
public:
	Skull(int _quantity = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;
	
private:
	Input* input;
};


#endif