#ifndef APPLE_H
#define APPLE_H
#include "Consumable.h"
#include "Input.h"
#include "GameObject.h"

class Apple : public Consumable {
public:
	Apple(Mesh* mesh, GameObject* player, int _quantity = 1, int stemLength = 1);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;
	
private:
	int stemLength;

	Input* input;
	GameObject* player;
};


#endif