#ifndef GHOST_H
#define GHOST_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"
#include "Inventory.h"

class Ghost : public Enemy {
public:
	Ghost();
	~Ghost();

	void Init(SceneBase* scene, Inventory* inventory);
	void Update(double dt);

private:
	enum GHOST_STATE {
		INACTIVE,
		FOLLOWING,
		HIDING,
		HOSTILE,

		GSTATE_TOTAL
	};

	GHOST_STATE state;

	SpriteAnimation* animatedSprites;

	Inventory* inventory;

};


#endif