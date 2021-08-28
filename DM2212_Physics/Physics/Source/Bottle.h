#ifndef BOTTLE_H
#define BOTTLE_H
#include "Consumable.h"
#include "Input.h"

class Bottle : public Consumable {
public:
	Bottle(int _quantity = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;
	
private:
	Input* input;
};


#endif