#ifndef REDSHELL_H
#define REDSHELL_H
#include "Consumable.h"
#include "Input.h"

class Redshell : public Consumable {
public:
	Redshell(int _quantity = 1); //constructor
	void Init() override; //init
	void Update(double dt) override; //update
	bool IsEqual(Item* item) override; //item
	
private:
	Input* input; //input
};


#endif