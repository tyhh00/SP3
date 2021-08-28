#ifndef REDSHELL_H
#define REDSHELL_H
#include "Consumable.h"
#include "Input.h"

class Redshell : public Consumable {
public:
	Redshell(int _quantity = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;
	
private:
	Input* input;
};


#endif