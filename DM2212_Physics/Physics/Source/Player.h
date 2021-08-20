#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Keyboard.h"
#include "Ability.h"
#include "Portal.h"
#include "Camera.h"
#include "SceneBase.h"
#include "GameObjectManager.h"

class Player : public GameObject {
public:
	~Player();

	void Init();
	void Update(double dt);
	void Render(SceneBase* scene);
	void CollidedWith(GameObject* go);
	void SetAbilities(Ability* a, Ability* b);

private:
	SpriteAnimation* animatedSprites;
	Mesh* portalSprite;

	float dashTimer;
	bool isDashing = false;

	bool invisibility;

	float speed;
	float jump_force;

	Ability* abilityArray[2];
	Keyboard* keyboard;

};


#endif