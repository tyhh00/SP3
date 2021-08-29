#ifndef TRIDENT_H
#define TRIDENT_H
#include "Weapon.h"
#include "Input.h"
#include "Camera.h"
#include "GameObjectManager.h"
#include "SpriteAnimation.h"
#include "ThunderBlock.h"

class TridentGO : public GameObject {
public:
	void CollidedWith(GameObject* go); //checks if collide with block
	TridentGO(); //trident go constructor
	~TridentGO(); //trident go destructor

	void Init(); //trident go init
	void Update(double dt); //trident go update
};

class Trident : public Weapon {
public:
	Trident(); //trident constructor
	~Trident(); //trident destructor

	void Init(Camera* cam, GameObjectManager* goManager, GameObject* target); //trident init
	void Update(double dt); //trident update
	bool IsEqual(Item* item1); //trident item
	void CollidedWith(GameObject* go); //checks if collide with block
	
private:

	//INPUT
	Input* input;
	
	//CAMERA
	Camera* camera;
	
	//GAME OBJECT MANAGER
	GameObjectManager* GOmanager;
	
	//MOUSE POSITION
	Vector3 mouseDownPos;
	
	//PLAYER
	GameObject* player;
	
	//SPRITE ANIMATION
	SpriteAnimation* animatedSprites;

	//TRIDENT GAME OBJECT
	TridentGO* tridentGO;

	//FUNCTIONS
	void CursorToWorldPosition(double& theX, double& theY); //mouse cursor to world position
	bool checkWithinScreen(); //checks if trident within screen. if not within screen, delete tridentGO
};


#endif