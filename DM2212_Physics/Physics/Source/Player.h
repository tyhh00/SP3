#ifndef PLAYER_H
#define PLAYER_H

#include "GameObject.h"
#include "SpriteAnimation.h"
#include "Physics.h"
#include "Input.h"
#include "Ability.h"
#include "SceneBase.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "Inventory.h"
#include "GameManager.h"

class Player : public GameObject {
public:

	enum MOVEMENT_MODE {
		WASD,
		PLATFORMER,

		M_TOTAL
	};

	Player();
	~Player();

	void Init(Camera* cam, MOVEMENT_MODE mode, GameObjectManager* GOM, Inventory* inventory);
	void Update(double dt);
	void Render(SceneBase* scene);
	void CollidedWith(GameObject* go);
	void SetAbilities(Ability* a, Ability* b);
	void UpdateMovement(double dt);
	float GetStamina();
	void DecreaseStamina(float amt);

	int dashDir = 0;
	float curr_max_vel;
	bool invisibility;
private:
	const float MAX_VEL = 30.f;
	const float MAX_SPRINTVEL = 45.f;

	MOVEMENT_MODE mode;
	SpriteAnimation* animatedSprites;

	Camera* cam;


	bool isDashing = false;

	float accel;
	float speed_multiplier;
	float jump_force;

	float stamina;
	float max_stamina;
	float staminaCD;

	float stamina_rate_multiplier;

	Ability* abilityArray[2];
	Input* input;
	GameObjectManager* goManager;
	GameManager* gameManager;
	Inventory* inventory;
};


#endif