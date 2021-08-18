#ifndef CHEESE_H
#define CHEESE_H
#include "Consumable.h"

class Cheese : public Consumable {
public:
	Cheese();
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;

private:
	
};


#endif