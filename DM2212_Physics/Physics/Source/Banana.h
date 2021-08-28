#ifndef BANANA_H
#define BANANA_H
#include "Consumable.h"
#include "GameObject.h"

class Banana : public Consumable {
public:
	Banana(Mesh* mesh, GameObject* _player);
	void Init() override;
	void Update(double dt) override;
	bool IsEqual(Item* item) override;

private:
	GameObject* player;
};


#endif