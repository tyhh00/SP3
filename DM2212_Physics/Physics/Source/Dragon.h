#ifndef DRAGON_H
#define DRAGON_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Enemy.h"
#include "Inventory.h"


class Dragon : public Enemy {
public:
	Dragon();
	~Dragon();

	void Init(SceneBase* scene, Vector3& target, int numParts, GameObjectManager* goManager); // keep target only if you need to access player pos
	void Update(double dt);
	void Render(SceneBase* scene);
	float convertidk(float pain);

private:
	enum ENEMY_STATE {
		INACTIVE, // default state, not moving
		SINCURVE,
		ATTACK,
		DEAD,
		GSTATE_TOTAL
	};

	float activeRange;
	
	ENEMY_STATE state;

	Vector3 *playerPos; // to keep track of player pos
	
	SpriteAnimation* animatedSprites;
	Inventory* inventory;
	GameObjectManager* goM;

	std::vector<GameObject*> dragon;
	float angle; 
	float curve;
	double curveTimer;
};


#endif