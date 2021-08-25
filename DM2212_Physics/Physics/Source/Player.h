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

	enum MOVEMENT_MODE {
		WASD,
		PLATFORMER,

		M_TOTAL
	};

	Player();
	~Player();

	void Init(MOVEMENT_MODE mode, GameObjectManager* GOM, Inventory* inventory);
	void Update(double dt);
	void Render(SceneBase* scene);
	void CollidedWith(GameObject* go);
	void SetAbilities(Ability* a, Ability* b);
	void UpdateMovement(double dt);
	float GetStamina();
	void DecreaseStamina(float amt);

private:
	const float MAX_VEL = 30.f;
	const float MAX_SPRINTVEL = 45.f;

	MOVEMENT_MODE mode;
	SpriteAnimation* animatedSprites;
	Mesh* portalSprite;
//	Mesh* livesIcon;
	Mesh* staminaBar;

	int dashDir = 0;
	bool isDashing = false;

	bool invisibility;

	float accel;
	float curr_max_vel;
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