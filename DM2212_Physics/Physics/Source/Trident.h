#ifndef TRIDENT_H
#define TRIDENT_H
#include "Weapon.h"
#include "Input.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "SpriteAnimation.h"

class TridentGO : public GameObject {
public:
	void CollidedWith(GameObject* go);
	TridentGO();
	~TridentGO();

	void Init();
	void Update(double dt);
};

class Trident : public Weapon {
public:
	Trident();
	~Trident();

	void Init(Camera* cam, GameObjectManager* goManager, Vector3& pos);
	void Update(double dt);
	bool IsEqual(Item* item1);
	void CollidedWith(GameObject* go);
	
private:
	Input* input;
	void CursorToWorldPosition(double& theX, double& theY);
	bool checkWithinScreen();
	Camera* camera;
	GameObjectManager* GOmanager;
	TridentGO* tridentGO;
	Vector3* playerPos;
	Vector3 mouseDownPos;
	SpriteAnimation* animatedSprites;
};


#endif