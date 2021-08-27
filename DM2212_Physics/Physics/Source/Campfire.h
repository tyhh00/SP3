#ifndef CAMPFIRE_H
#define CAMPFIRE_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
#include "Ability.h"
#include "Inventory.h"

class Campfire : public GameObject{

private:
	SpriteAnimation * campfireSprite;

	SceneBase* scene;
	Inventory* inv;
	Vector3* playerPos;

	bool isLit;
	float litTimer;
public:
	Campfire();
	~Campfire();

	void Init(SceneBase* scene, Inventory* inv, Vector3& playerPos);
	void Update(double dt);
	void SetAnimation(std::string anim_name, int repeat, double anim_time);

	bool GetIsLit();
};

#endif