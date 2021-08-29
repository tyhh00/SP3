#ifndef BLUESHELL_H
#define BLUESHELL_H
#include "Consumable.h"
#include "Input.h"

class Blueshell : public Consumable {
public:
	Blueshell(int _quantity = 1);//constructor
	void Init() override; //init
	void Update(double dt) override; //update
	bool IsEqual(Item* item) override; //item

private:
	Input* input; //input
};


#endif