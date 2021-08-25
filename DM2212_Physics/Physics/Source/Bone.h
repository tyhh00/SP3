#ifndef BONE_H
#define BONE_H
#include "Consumable.h"
#include "Input.h"

class Bone : public Consumable {
public:
	Bone(int _quantity = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;
	
private:
	Input* input;
};


#endif