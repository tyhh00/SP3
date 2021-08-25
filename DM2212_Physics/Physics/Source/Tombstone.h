#ifndef TOMBSTONE_H
#define TOMBSTONE_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"
#include "Inventory.h"


class Tombstone : public GameObject {
public:
	Tombstone();
	~Tombstone();

	void Init(SceneBase* scene, Inventory* inventory);
	void Update(double dt);

private:

	
	Inventory* inventory;

	bool isWithinFlashlight();
	bool isBeingAttacked();

};


#endif