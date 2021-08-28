#ifndef BLUESHELL_H
#define BLUESHELL_H
#include "Consumable.h"
#include "Input.h"

class Blueshell : public Consumable {
public:
	Blueshell(int _quantity = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;
	
private:
	Input* input;
};


#endif