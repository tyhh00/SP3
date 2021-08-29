#ifndef BOTTLE_H
#define BOTTLE_H
#include "Consumable.h"
#include "Input.h"

class Bottle : public Consumable {
public:
	Bottle(int _quantity = 1);//constructor
	void Init() override; //init
	void Update(double dt) override; //update
	bool IsEqual(Item* item) override; //item

private:
	Input* input; //input
};


#endif