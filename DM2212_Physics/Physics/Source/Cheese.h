#ifndef CHEESE_H
#define CHEESE_H
#include "Consumable.h"

class Cheese : public Consumable {
public:
	Cheese(Mesh* mesh, int numOfHoles = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;

private:
	int numOfHoles;
};


#endif