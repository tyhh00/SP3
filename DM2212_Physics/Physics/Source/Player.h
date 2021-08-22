#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Ability.h"
#include "Portal.h"
#include "Dash.h"
#include "Grappling.h"
#include "SceneBase.h"
#include "GameObjectManager.h"
#include "Inventory.h"

class Player : public GameObject {
public:
	Player();
	~Player();

	void Init(GameObjectManager* GOM, Inventory* inventory);
	void Update(double dt);
	void Render(SceneBase* scene);
	void CollidedWith(GameObject* go);
	void SetAbilities(Ability* a, Ability* b);

	float GetStamina();
	void DecreaseStamina(float amt);

private:
	/*const float ACCEL_SPEED = 100.f;
	const float MAX_ACCEL = 5.f;
	const float MAX_ACCEL_VEL = 30.f;
	const float MAX_VEL = 100.f;*/

	SpriteAnimation* animatedSprites;
	Mesh* portalSprite;

	float accel;
	bool isDashing = false;

	bool invisibility;
	bool AkeyDown;
	bool DkeyDown;

	float speed;
	float speed_multiplier;
	float jump_force;

	float stamina;
	float max_stamina;

	float stamina_rate_multiplier;

	Ability* abilityArray[3];
	Input* input;
	GameObjectManager* goManager;
	Inventory* inventory;

};


#endif